// 2023/3/13
// zhangzhong
// 2d array

#ifndef ARRAY_2D
#define ARRAY_2D

#include <malloc.h>
#include <stddef.h>
#include <string.h>

typedef struct {
    int* data;
    size_t row;
    size_t col;
} array2d_t;

static array2d_t make_array2d(size_t row, size_t col, int initial) {
    int* data = malloc(row * col * sizeof(int));
    memset(data, initial, row * col * sizeof(int));
    array2d_t array = {
        data,
        row,
        col,
    };
    return array;
}

static void free_array2d(array2d_t* array) {
    if (array) {
        free(array->data);
        array->data = NULL;
        array->row = 0;
        array->col = 0;
    }
}

static int* array2d_at(const array2d_t* array, size_t i, size_t j) {
    return array->data + i * array->col + j;
}

#endif // ARRAY_2D
