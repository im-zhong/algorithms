// 2023/3/20
// zhangzhong
// lzw compression and expansion

#include "compress/lzw.h"
#include "container/vector.h"
#include "string/trie.h"
#include "util/bitbuf.h"
#include "util/c_string.h"
#include "util/file.h"
#include <stdint.h>
#include <stdio.h>

static vector_t vector;

// csapp: datalab how many bits 提供了一个二分搜索的思路
static int minimum_bits(uint64_t n) {
    int bits = 0;
    while (n != 0) {
        bits++;
        n >>= 1U;
    }
    return bits;
}

int lzw_compression(const char* input, const char* output) {
    FILE* infp = open_file_for_read(input);
    // 一开始先不写入文件 先将所有的coding 编码 写入vector
    // 然后确定能包括所有的coding的最小的bit数
    // 然后将bit数写入文件
    // 然后将所有的coding写入文件

    string_t str = make_string("");
    trie_t trie;
    vector_t codings = make_vector(0);
    make_trie(&trie);

    // 不对 我们得先初始化trie
    // trie得包含所有的ASCII字符
    char ascii[2] = {'\0', '\0'};
    for (int i = 0; i < ASCII_SIZE; i++) {
        // 这是个问题 向trie tree 插入 \0 会怎么样
        ascii[0] = (char)i;
        // 这样写太危险了
        trie_insert_sequence(&trie, ascii, 1, i);
        printf("insert to trie: %d\n", i);
    }

    value_t prev_coding = 0;
    value_t coding = ASCII_SIZE;
    char byte = 0;
    // while (fread(&byte, 1, 1, infp) == 1) {
    //     // 只有读到1的时候才是正常的
    //     // do something
    //     prev_coding = 0;
    //     string_pushback(&str, byte);
    //     if (trie_search(&trie, str.data, &prev_coding)) {
    //         // keep reading
    //     } else {
    //         // 将prev coding写入vector
    //         // 将str写入trie
    //         vector_push_back(&codings, prev_coding);
    //         trie_insert(&trie, str.data, coding);
    //         // 每写入一个新的coding coding就要加1
    //         coding++;
    //         // 然后创建新的str 只包含最后读到的这个字符
    //         string_clear(&str);
    //         string_pushback(&str, byte);
    //     }
    // }
    // 结束之后 str里面放着最后一个符号

    bool is_eof = false;
    // 不对不对 这样写逻辑不对
    // 我们应该一直读 直到读到不认识的字符串序列为止
    while (true) {
        is_eof = false;
        while (string_empty(&str) ||
               trie_search_sequence(&trie, str.data, str.size, &prev_coding)) {
            if (fread(&byte, 1, 1, infp) != 1) {
                is_eof = true;
                break;
            }
            string_pushback(&str, byte);
        }

        // 哪怕是读到了eof 我们也还是要把最后一个coding写入vector
        // 将prev coding写入vector
        // 将str写入trie
        // prev_coding 不应该是零
        assert(prev_coding != 0);
        vector_push_back(&codings, prev_coding);
        trie_insert_sequence(&trie, str.data, str.size, coding);
        // 每写入一个新的coding coding就要加1
        coding++;
        // 然后创建新的str 只包含最后读到的这个字符
        string_clear(&str);
        string_pushback(&str, byte);
        // 我们有可能读到EOF
        if (is_eof) {
            // 那么我们将最后一个coding写入vector
            break;
        }
    }

    (void)fclose(infp);
    free_string(&str);

    // 然后我们要确定能包括所有的coding的最小的bit数
    int bits = minimum_bits(coding);
    // 开始写入文件
    FILE* outfp = open_file_for_write(output);
    // 首先将一个coding的bit数写入文件
    // 因为最长才是64bit 所以一个byte就够了
    // 创建bitbuf outbuf
    bitbuf_t* outbuf = make_bitbuf(outfp);
    bitbuf_write_byte(outbuf, bits);

    // important！！！
    // bitbuf是没法正常的判断文件是否结束的 因为bit的数量是随机的
    // 而文件系统提供的抽象是以byte为单位 所以很大概率是对不齐的
    // 所以向Huffman lzw这样的压缩算法都需要在压缩文件中写入编码coding的数量
    bitbuf_write_len(outbuf, codings.size);

    // 然后遍历vecotr 将所有的coding写入文件
    for (size_t i = 0; i < codings.size; i++) {
        // todo(zhangzhong): 我确实应该给vector提供个好的接口 vector_size,
        // vector_at 这样的 bitbuf_write_bits(outbuf, vector_at(&codings, i),
        // bits);
        bitbuf_write_integer(outbuf, codings.data[i], bits);
    }
    bitbuf_flush(outbuf);

    free_bitbuf(outbuf);
    (void)fclose(outfp);

    // 测试 暂时不释放
    // free_vector(&codings);
    vector = codings;
    for (size_t i = 0; i < vector.size; i++) {
        printf("%lu ", vector.data[i]);
        assert(vector.data[i] != 0);
    }
    return 0;
}

int lzw_expansion(const char* input, const char* output) {

    vector_t codings = make_vector(0);
    uint64_t coding = 0;
    // 先初始化符号表
    // vector中应该有ASCII_SIZE个元素
    // vector的元素应该是string_t
    // 不行 make_xxx 应该返回指针
    // 这样就可以把他们放到容器里面了
    // 否则还得先malloc 然后再放到容器里面
    // todo(zhangzhong): 想明白了 所有的make都应该返回指针
    // todo(zhangzhong): 把所有的通用容器用void*替换掉
    for (int i = 0; i < ASCII_SIZE; i++) {
        char ascii[2] = {'\0', '\0'};
        ascii[0] = (char)i;
        string_t str = make_string(ascii);
        string_t* pstr = malloc(sizeof(string_t));
        *pstr = str;
        vector_push_back(&codings, (value_t)pstr);
    }
    // 我们在读文件的时候 还需要向codings中添加编码
    // 所以codings.size表示的就是当前的编码个数

    FILE* infp = open_file_for_read(input);
    FILE* outfp = open_file_for_write(output);
    bitbuf_t* inbuf = make_bitbuf(infp);
    // 先读取一个byte
    // 这个byte就是一个coding的bit数
    // 然后读取所有的coding
    unsigned int coding_length = 0;
    assert(bitbuf_read_byte(inbuf, &coding_length) == 0);

    size_t len = 0;
    assert(bitbuf_read_len(inbuf, &len) == 0);

    // test
    // 现在验证写进去的coding和读出来的一样
    size_t coding_index = 0;

    uint64_t prev_coding = 0;
    // 然后读取所有的coding
    // 从概念上理解 这个过程是非常简单的
    // 就是循环遍历文件 读到一个编码 就输出此编码对应的字符序列
    // 还要处理的一件事情就是重建符号表
    // 每一步都将当前的coding的首字母和prev coding对应的字符串拼接起来
    // 加入符号表
    // 但是在最开始的时候 是不应该建立符号的
    // 所以我们的先读一个integer
    assert(bitbuf_read_integer(inbuf, &coding, coding_length) == 0);
    assert(coding != 0);
    assert(coding == vector.data[coding_index]);
    coding_index++;
    string_t* pstr = (string_t*)codings.data[coding];
    // todo(zhangzhong): 因为c语言的字符串以0结尾 所以给实现带来了许多困难
    // 真正的文件中是可以存在许多许多零的
    // 我们真正需要的是一个byte sequence 而不是string
    // 应该是 vector<byte>
    // 先暂时让随机数据不会出现零吧
    assert(pstr->size > 0);
    assert(fwrite(pstr->data, 1, pstr->size, outfp) == pstr->size);
    prev_coding = coding;
    // 讲道理 我们是不可能读到零的

    printf("\n\n\n\n\n");
    while (bitbuf_read_integer(inbuf, &coding, coding_length) == 0) {
        // 然后根据coding找到对应的字符串
        // 然后写入文件
        printf("%lu ", coding);
        assert(coding == vector.data[coding_index]);
        coding_index++;
        assert(coding != 0);
        // 这里分成两种情况
        // 一种是读到的编码是我们直到的
        if (coding < codings.size) {
            // 然后将此coding对应的字符串写入文件
            string_t* pstr = (string_t*)codings.data[coding];
            assert(pstr->size > 0);
            assert(fwrite(pstr->data, 1, pstr->size, outfp) == pstr->size);
            // 然后将当前coding对应的字符串的第一个字符和prev
            // coding对应的字符串拼接起来
            // 然后将这个字符串加入到codings中
            string_t* prev_str = (string_t*)codings.data[prev_coding];
            string_t new_str = make_string(prev_str->data);
            string_pushback(&new_str, pstr->data[0]);
            string_t* pnew_str = malloc(sizeof(string_t));
            *pnew_str = new_str;
            vector_push_back(&codings, (value_t)pnew_str);
        } else {
            // 特殊情况 ABA
            // 我们读到了一个不认识的符号
            // 但是这个编码可以通过prev coding构造出来
            // 所以我们要先构造出来
            // 然后再写入文件
            string_t* prev_str = (string_t*)codings.data[prev_coding];
            // 上面的分支拿到look ahead char
            // 是当前的conding对应的字符串的第一个字符 但是现在我们的look ahead
            // char 就是prev conding对应的字符串的第一个字符
            string_t new_str = make_string(prev_str->data);
            string_pushback(&new_str, prev_str->data[0]);
            string_t* pnew_str = malloc(sizeof(string_t));
            *pnew_str = new_str;
            vector_push_back(&codings, (value_t)pnew_str);
            // 然后将这个字符串写入文件
            assert(fwrite(new_str.data, 1, new_str.size, outfp) ==
                   new_str.size);
        }

        prev_coding = coding;
        if (coding_index == len) {
            break;
        }
    }

    free_bitbuf(inbuf);
    (void)fclose(infp);
    (void)fclose(outfp);
    // free string
    for (size_t i = 0; i < codings.size; i++) {
        string_t* pstr = (string_t*)codings.data[i];
        free_string(pstr);
        free(pstr);
    }
    free_vector(&codings);
    return 0;
}

// file scope tests

static void test_minimum_bits() {
    assert(minimum_bits(0) == 0);
    assert(minimum_bits(1) == 1);
    assert(minimum_bits(2) == 2);
    assert(minimum_bits(3) == 2);
    assert(minimum_bits(4) == 3);
}

void test_lzw() { test_minimum_bits(); }
