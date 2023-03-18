// 2023/3/18
// zhangzhong
// symmetric matrix
// m(i, j) == m(j, i)

#ifndef CONTAINER_SYMMETRIC_MATRIX_H
#define CONTAINER_SYMMETRIC_MATRIX_H

#include "def.h"
#include <assert.h>
#include <stdlib.h>

typedef struct {
    value_t* data;
    size_t size;
    size_t dim;
} symmetric_matrix_t;

// 构造一个对称矩阵 维度为dim
static symmetric_matrix_t make_symmetric_matrix(size_t dim) {
    assert(dim > 0);
    symmetric_matrix_t m;
    m.dim = dim;
    // size = (1 + dim) * dim / 2
    m.size = (1 + dim) * dim / 2;
    m.data = malloc(sizeof(value_t) * m.size);
    return m;
}

static value_t* symmetric_matrix_at(const symmetric_matrix_t* m, size_t i,
                                    size_t j) {
    assert(i < m->dim && j < m->dim);
    // i <= j, 保证矩阵对称 要上半矩阵
    if (i > j) {
        size_t t = i;
        i = j;
        j = t;
    }
    // i <= j
    // index = (dim + ... + dim - i + 1) + (j - i)
    // index = i * (2*dim - i + 1) / 2 + (j - i)
    size_t index = i * (2 * m->dim - i + 1) / 2 + (j - i);
    assert(index < m->size);
    return m->data + index;
}

static void free_symmetric_matrix(symmetric_matrix_t* m) {
    if (m) {
        free(m->data);
        m->data = NULL;
        m->size = 0;
    }
}

#endif // CONTAINER_SYMMETRIC_MATRIX_H
