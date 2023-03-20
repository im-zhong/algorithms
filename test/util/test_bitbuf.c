// 2022/12/10
// zhangzhong
// test bit buffer

#include "container/vector.h"
#include "util/bitbuf.h"
#include "util/c_random.h"
#include "util/file.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// 怎么测呢?? 准备一个随机数组
#define NUM (4096)
// 不对 不对 我想起来了
// 这个数组就保存0或者1
int bits[NUM];
// 生成随机数即可

void test_bitbuf_write(char* filename) {
    // 先测试写 再测试读
    // 生成很多个随机的bit
    // 写进去再读出来 要求一摸一样

    // 生成随机bit
    for (int i = 0; i < NUM; i++) {
        bits[i] = rrand(0, 1);
    }

    FILE* stream = fopen(filename, "wb");
    if (stream == NULL) {
        printf("open %s failed.\n", filename);
        exit(EXIT_FAILURE);
    }

    bitbuf_t* bf = make_bitbuf(stream);
    for (int i = 0; i < NUM; i++) {
        bitbuf_write_bit(bf, bits[i]);
    }
    bitbuf_flush(bf);

    free_bitbuf(bf);
    fclose(stream);
}

void test_bitbuf_read(char* filename) {
    FILE* stream = fopen(filename, "rb");
    if (stream == NULL) {
        printf("open %s failed.\n", filename);
        exit(EXIT_FAILURE);
    }

    // 读取文件并验证是否一致
    bitbuf_t* bf = make_bitbuf(stream);
    int bit = 0;
    for (int i = 0; i < NUM; i++) {
        assert(bitbuf_read_bit(bf, &bit));
        assert(bit == bits[i]);
    }
    printf("\n");

    free_bitbuf(bf);
    fclose(stream);
}

void test_bitbuf_integer() {
    int bit_length = 12;
    uint64_t value = 0;
    vector_t values = make_vector(0);
    const int MAX = 1000000;

    FILE* outfp = open_file_for_write("test_bitbuf_integer.txt");
    bitbuf_t* outbuf = make_bitbuf(outfp);
    for (int i = 0; i < MAX; i++) {
        value = rrand('a', 'z');
        vector_push_back(&values, (value_t)value);
        bitbuf_write_integer(outbuf, value, bit_length);
    }
    bitbuf_flush(outbuf);
    free_bitbuf(outbuf);
    (void)fclose(outfp);

    FILE* infp = open_file_for_read("test_bitbuf_integer.txt");
    bitbuf_t* inbuf = make_bitbuf(infp);
    uint64_t read_value = 0;
    for (int i = 0; i < MAX; i++) {
        value = (uint64_t)values.data[i];
        bitbuf_read_integer(inbuf, &read_value, bit_length);
        assert(value == read_value);
    }

    // 在这里我们应该已经读完了所有的文件
    // 所以我们需要检查一下是否已经读完
    // 果然 这里出错了
    // bitbuf没法判断文件是否结束 所以压缩的时候必须要写入coding的数量
    // 可以使用 bitbuf_read/write_len
    // assert(bitbuf_read_integer(inbuf, &read_value, bit_length) != 0);

    free_bitbuf(inbuf);
    (void)fclose(infp);
}

int main(int argc, char* argv[]) {

    // 准备随机数组
    char* filename = "test_bitbuf.bin";
    test_bitbuf_write(filename);
    test_bitbuf_read(filename);
    test_bitbuf_integer();
}
