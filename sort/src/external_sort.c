// 2023/3/13
// zhangzhong
// external sort

#include "sort/external_sort.h"
#include "container/index_heap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    FILE* fp;
    int num;
} ordered_file_t;

int intcmp(const void* left, const void* right) {
    return *(int*)left - *(int*)right;
}

bool icmp(const void* left, const void* right) {
    return ((ordered_file_t*)left)->num < ((ordered_file_t*)right)->num;
}

static void check_array_order(const int* array, size_t size) {
    for (size_t i = 1; i < size; i++) {
        assert(array[i - 1] <= array[i]);
    }
}

static void check_file_order(const char* filename) {
    // 读取数据 并且验证
    FILE* in = fopen(filename, "r");
    int curr_num = -1;
    int num = 0;
    while (fread(&num, sizeof(int), 1, in) != 0) {
        assert(curr_num <= num);
        curr_num = num;
    }
    (void)fclose(in);
}

void external_sort(const char* input, const char* output) {

    const size_t chunk_size = 1024;

    FILE* in = fopen(input, "r");

    // 我们一次读取一定数量的数据
    int buffer[chunk_size];

    char tmpfile[16] = "tmp";

    int i = 0;
    while (true) {
        size_t r = fread(buffer, sizeof(int), chunk_size, in);
        if (r == 0) {
            // read doen of end of file
            break;
        }

        // 在这里我们要对buffer进行一波排序
        qsort(buffer, chunk_size, sizeof(int), intcmp);
        // 然后将排完序的写出到一个文件中
        // c语言 to_string 还真是麻烦
        check_array_order(buffer, chunk_size);

        int n = sprintf(tmpfile, "tmp%d.txt", i);
        tmpfile[n] = '\0';
        i++;

        FILE* out = fopen(tmpfile, "w");
        // 读多少 写多少
        assert(fwrite(buffer, sizeof(int), chunk_size, out) == r);
        (void)fclose(out);
    }

    (void)fclose(in);
    assert(i <= chunk_size);
    // 在这里检测所有的文件都是有序的
    size_t file_num = i;

    for (size_t i = 0; i < file_num; i++) {
        int n = sprintf(tmpfile, "tmp%zu.txt", i);
        tmpfile[n] = '\0';
        check_file_order(tmpfile);
    }

    // 然后我们就需要开始k路归并了
    // 那么首先为了简单 我们可以假设 k <= chunk_size

    ordered_file_t files[chunk_size];
    // 打开所有的文件
    // int file_num = i;
    for (int i = 0; i < file_num; i++) {
        int n = sprintf(tmpfile, "tmp%d.txt", i);
        assert(n > 0);
        tmpfile[n] = '\0';

        // 打开文件句柄的数量上限了 。。。 无语
        FILE* in = fopen(tmpfile, "r");
        // assert(in);
        if (!in) {
            printf("%s\n", tmpfile);
            assert(in);
        }
        files[i].fp = in;
        // 因为我们生成的数据是大于零的 这里方便调试
        files[i].num = -1;
        // nonono 不能再这里close
        // 我们要全部排完序之后才能close
        // (void)fclose(in);
    }

    iheap_t heap;
    iheap_init(&heap, files, chunk_size, sizeof(ordered_file_t), chunk_size,
               icmp);

    // 然后就是遍历所有的文件 将所有的数据插入堆中
    for (int i = 0; i < file_num; i++) {
        size_t r = fread(&files[i].num, sizeof(int), 1, files[i].fp);
        if (r == 0) {
            // read eof, so we can ignore it
            continue;
        }
        // 我们读到了一个数据 将其插入堆中
        iheap_push(&heap, i);
    }

    // 打开输出文件
    FILE* out = fopen(output, "w");
    while (!iheap_is_empty(&heap)) {
        // 找出当前最小的一个
        size_t i = iheap_top(&heap);
        // 将其写入output文件中
        assert(fwrite(&files[i].num, sizeof(int), 1, out) == 1);
        // 然后将这个数据pop
        iheap_pop(&heap);
        // 然后从top对应的文件中再次读取出一个数据
        size_t r = fread(&files[i].num, sizeof(int), 1, files[i].fp);
        if (r != 0) {
            iheap_push(&heap, i);
        }
    }

    // 将所有打开的文件关闭
    for (int i = 0; i < file_num; i++) {
        (void)fclose(files[i].fp);
    }
    (void)fclose(out);
}

// 为了验证两个文件是一样的 我们输入输出文件都要 还要给定数据的大小
bool check_external_order(const char* random_file, const char* ordered_file,
                          size_t num) {

    // 那么这样我们就需要准备两个数组
    int* randoms = malloc(num * sizeof(int));
    // 然后从random file中读取所有数据
    FILE* in = fopen(random_file, "r");
    assert(in);
    size_t n = fread(randoms, sizeof(int), num, in);
    assert(n == num);
    // 这个时候再去读 读到的应该就是EOF
    assert(fread(randoms, sizeof(int), 1, in) == 0);
    (void)fclose(in);

    // 然后堆这个数组进行排序
    qsort(randoms, num, sizeof(int), intcmp);
    // 标注库的函数我就不用再测试了

    // 然后把ordered_file文件读出来
    int* ordered = malloc(num * sizeof(int));
    in = fopen(ordered_file, "r");
    assert(in);
    n = fread(ordered, sizeof(int), num, in);
    assert(n == num);
    // 这个时候再去读 读到的应该就是EOF
    assert(fread(ordered, sizeof(int), 1, in) == 0);
    (void)fclose(in);
    // qsort(ordered, num, sizeof(int), intcmp);

    // 然后就是激动人心的一刻 我们对比两个数组是否完全相同
    for (size_t i = 0; i < num; i++) {
        assert(randoms[i] == ordered[i]);
    }

    free(randoms);
    free(ordered);
    return true;
}
