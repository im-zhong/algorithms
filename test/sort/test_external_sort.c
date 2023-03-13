// 2023/3/13
// zhangzhong
// test external sort

#include "sort/external_sort.h"
#include "util/c_random.h"
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

void test_small_external_sort() {
    const size_t num = 1024UL;
    const size_t size = num * sizeof(int);

    const char* random_file = "randoms.txt";
    FILE* out = fopen(random_file, "w");
    for (size_t i = 0; i < num; i++) {
        int n = rrand(0, INT_MAX);
        assert(fwrite(&n, sizeof(int), 1, out) == 1);
    }
    assert(fclose(out) == 0);

    // assert(check_external_order(random_file, random_file, num));

    const char* sorted_file = "sorted.txt";
    external_sort(random_file, sorted_file);
    assert(check_external_order(random_file, sorted_file, num));
}

void test_external_sort() {
    // 1. 生成随机数据的大文件

    // 4B
    // const int int_size = sizeof(int);
    // 你想要多少个数据吧
    // MB
    // 寄了  用一个小一点的测试用例吧
    const size_t num = 1024UL * 1024UL;
    const size_t size = num * sizeof(int);

    const char* random_file = "randoms.txt";
    FILE* out = fopen(random_file, "w");
    for (size_t i = 0; i < num; i++) {
        int n = rrand(0, INT_MAX);
        assert(fwrite(&n, sizeof(int), 1, out) == 1);
    }
    assert(fclose(out) == 0);

    const char* sorted_file = "sorted.txt";
    external_sort(random_file, sorted_file);
    assert(check_external_order(random_file, sorted_file, num));
}

int main() {
    test_small_external_sort();
    test_external_sort();
    return 0;
}
