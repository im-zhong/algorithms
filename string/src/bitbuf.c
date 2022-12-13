// 2022/12/10
// zhangzhong
// bitbuf

#include "string/bitbuf.h"
#include "container/vector.h"
#include "util/c_string.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bitbuf_t* make_bitbuf(FILE* stream) {
    bitbuf_t* bf = (bitbuf_t*)malloc(sizeof(bitbuf_t));
    bf->stream = stream;
    bf->n = 0;
    bf->offset = 0;
    return bf;
}

void free_bitbuf(bitbuf_t* bf) {
    if (bf) {
        free(bf);
    }
}

// read会过来调is_empty
// offset对于读和写的意义是不同的
// 写：在写之前 offset指向将要写入的位置
// 读：在读之前 offset指向将要读取的位置
static inline bool bitbuf_is_empty(bitbuf_t* bf) {
    // 这样我们就知道是不是空的了
    // 我懂了 就是这样
    // 写入和读取的offset的移动模式是一样的
    // 只不过他们的动作含义不一样
    // 读：读取offset指向的位置 然后offset++ 表示下一次我们要读取下一个位置
    // 但是cache的时候我们可能不会把缓存填满 所以判断读完的时候 需要和n进行判断
    // 但是写一定会写满在flush 所以write判断is_full就跟BITBUF_SIZE判断
    return bf->offset == bf->n * 8;
}

// write会过来掉 is_full
static inline bool bitbuf_is_full(bitbuf_t* bf) {

    // 比如我们的n = 1
    // 那么offset在写入八个bit之后就满了 也就是 offset == n * 8
    // offset = bf->n * 8
    // 不对 这里思考错了 is_full只会在写
    // 读流程和写流程不会同时发生
    return bf->offset == BITBUF_SIZE * 8UL;
}

// n = 0 - 7
// 把一个byte当作一个八个bit的数组来看待
// most significant bit = 7
// least significant bit = 0
static inline int read_bit(unsigned char c, size_t n) {
    assert(n < 8);
    // 使用 & 操作读取一个bit吧
    return (c & (1U << n)) >> n;
}

static inline int write_bit(unsigned char c, size_t n, int bit) {
    assert(n < 8);
    // 可能是set 也可能是clear
    // 总之就是让byte[n] = bit
    if (bit) {
        // bit = 1
        // 用或操作
        return c | (1U << n);
    }
    // bit = 0
    // 用与操作
    return c & ~(1U << n);
}

int bitbuf_cache(bitbuf_t* bf) {
    // 从stream中读取内容填充缓存
    // 还是读char更好处理一些
    // 这样nread返回的就是读取了多少byte
    size_t nread = fread(bf->buf, sizeof(char), BITBUF_SIZE, bf->stream);
    bf->n = nread * sizeof(char);
    // 这个逻辑好像不太对
    // 不太对啊 cache完之后 我们仍然要从offset = 0 开始读
    bf->offset = 0;
    if (nread == 0) {
        // 说明读到了结尾 可能是错误 可能是eof 需要额外判断
        if (ferror(bf->stream)) {
            // error
            exit(EXIT_FAILURE);
        }
        if (feof(bf->stream)) {
            // 文件正常结束
            return EOF;
        }
    }
    return 0;
}

// 将目前缓存内的bit写入底层文件中
int bitbuf_flush(bitbuf_t* bf) {
    int i = bf->offset / 8;
    int j = bf->offset % 8;
    int n = i;
    if (j != 0) {
        // todo
        // 最后一位的前面补零
        // 怎么补??
        // 其实就是 0 | 低j位
        unsigned int mask = 1U << (j + 1U);
        mask--;
        // 现在mask就是 j个1
        bf->buf[i] = bf->buf[i] & mask;
        n++;
    }

    // flush之后还没有读取任何内容时
    // 1. 这里不应该修改 bf->n write跟n没有任何关系
    // bf->n = 0;
    bf->offset = 0;

    // 如果j == 0 那么元素数量就是i
    // 如果 j != 0 元素数量是i+1
    return fwrite(bf->buf, sizeof(char), n, bf->stream);
}

int bitbuf_read_bit(bitbuf_t* bf, int* bit) {
    // 从文件中读取内容到缓存也应该单独包装成一个函数
    if (bitbuf_is_empty(bf)) {
        if (bitbuf_cache(bf) == EOF) {
            // 说明我们读取到了EOF
            return EOF;
        }
    }

    // 然后我们就可以从
    // 怎么从正确的位置读出数据呢
    // 或者说文字的bit流的正确位置在哪里呢??

    // 然后我们怎么找到正确的读取位置呢
    int i = bf->offset / 8;
    // byte内部的索引是
    int j = bf->offset % 8;
    // 就酱

    *bit = read_bit(bf->buf[i], j);
    bf->offset++;
    // 但是offset的最终位置在哪??
    return 1;
}

int bitbuf_write_bit(bitbuf_t* bf, int bit) {
    // 我们必须维护一个offset
    // 写入文件的时候必须按照8的倍数
    // 也就是flush的时候需要补零
    // 那么什么时候需要flush呢??
    // 当写入数据充满缓存的时候
    // 或者当用户主动调用flush的时候
    int i = bf->offset / 8;
    int j = bf->offset % 8;
    bf->buf[i] = write_bit(bf->buf[i], j, bit);
    bf->offset++;

    if (bitbuf_is_full(bf)) {
        bitbuf_flush(bf);
    }
    return 0;
}

int bitbuf_read_byte(bitbuf_t* bf, unsigned int* byte) {
    // 这个倒是没什么难的 就是调用八次read而已
    // 不过也要注意处理错误
    int bit = 0;
    *byte = 0;
    for (int i = 0; i < 8; i++) {
        // 如果读取到了末尾 直接返回失败
        // 这是不应该出现的情况
        if (!bitbuf_read_bit(bf, &bit)) {
            return -1;
        }

        *byte = write_bit(*byte, i, bit);
    }
    return 0;
}

int bitbuf_write_byte(bitbuf_t* bf, unsigned int byte) {
    for (int i = 0; i < 8; i++) {
        bitbuf_write_bit(bf, read_bit(byte, i));
    }
    return 0;
}

int bitbuf_read_len(bitbuf_t* bf, uint64_t* len) {
    // 其实就是读取八个byte
    uint64_t l;
    *len = 0;
    unsigned int byte = 0;
    for (int i = 0; i < 8; i++) {
        bitbuf_read_byte(bf, &byte);
        *len |= ((uint64_t)byte << (i * 8));
    }
    return 0;
}

int bitbuf_write_len(bitbuf_t* bf, uint64_t len) {
    // write len其实就是write byte
    // 我们同样按照 从低位开始写的策略
    for (int i = 0; i < 8; i++) {
        // 依次取低八位 写入
        unsigned int byte = len & 0xff;
        bitbuf_write_byte(bf, byte);
        // 然后len右移八位 下次循环就可以取第八位了 实际是第二个byte
        len >>= 8;
    }
    return 0;
}

// 不对啊 你想读多少字节呢??
int bitbuf_read_vector(bitbuf_t* bf, vector_t* str, size_t n) {
    int bit = 0;
    for (size_t i = 0; i < n; i++) {
        bitbuf_read_bit(bf, &bit);
        assert(bit == 0 || bit == 1);
        vector_push_back(str, bit);
    }
    return 0;
}

int bitbuf_write_vector(bitbuf_t* bf, vector_t str, size_t n) {
    int bit = 0;
    for (size_t i = 0; i < n; i++) {
        // 傻？？为什么要pop???
        // 不是顺序把字符写进去吗
        bit = str.data[i];
        assert(bit == 0 || bit == 1);
        bitbuf_write_bit(bf, bit);
    }
    return 0;
}
