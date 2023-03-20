// 2023/3/19
// zhangzhong
// file utility

#include "util/file.h"
#include "util/c_random.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

FILE* open_file_for_read(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        (void)fprintf(stderr, "open %s for read failed.\n", filename);
        exit(EXIT_FAILURE);
    }
    return fp;
}

FILE* open_file_for_write(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        (void)fprintf(stderr, "open %s for write failed.\n", filename);
        exit(EXIT_FAILURE);
    }
    return fp;
}

int generate_random_file(const char* filename, size_t size) {
    FILE* outfp = open_file_for_write(filename);
    for (size_t i = 0; i < size; i++) {
        char byte = (char)rrand('a', 'z');
        if (fwrite(&byte, 1, 1, outfp) != 1) {
            (void)fprintf(stderr, "write to %s failed.\n", filename);
            return -1;
        }
    }
    (void)fclose(outfp);
    return 0;
}
