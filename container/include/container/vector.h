// 2022/11/18
// zhangzhong
// vector

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "def.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    value_t* data;
    size_t size;
    size_t capacity;
} vector_t;

static inline vector_t make_vector(size_t capacity) {
    vector_t vector;
    vector.data = NULL;
    vector.size = 0;
    vector.capacity = capacity;
    if (capacity > 0) {
        // 分配空间
        vector.data = (value_t*)malloc(capacity * sizeof(value_t));
        assert(vector.data);
    }
    return vector;
}

static inline void free_vector(vector_t* vector) {
    assert(vector);

    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

static inline void vector_clear(vector_t* vector) { vector->size = 0; }

static inline void vector_realloc(vector_t* vector) {
    assert(vector);

    if (vector->capacity == 0) {
        vector->capacity = 4;
    }

    vector->capacity <<= 1U;
    vector->data =
        (value_t*)realloc(vector->data, vector->capacity * sizeof(value_t));
}

static inline vector_t copy_vector(vector_t* vector) {
    assert(vector);
    vector_t copy = make_vector(vector->capacity);
    copy.size = vector->size;
    copy.capacity = vector->capacity;
    // 复制数据
    if (copy.data && vector->data) {
        memcpy(copy.data, vector->data, vector->size * sizeof(value_t));
    }
    return copy;
}

// 非常难用
// 还不如就写一个的int的vector呢
// 虽然也没用
static inline void vector_push_back(vector_t* vector, value_t element) {
    assert(vector);

    if (vector->size + 1 > vector->capacity) {
        vector_realloc(vector);
    }

    (vector->data)[vector->size] = element;
    ++(vector->size);
}

static inline value_t vector_pop_back(vector_t* vector) {
    assert(vector);
    if (vector->data == NULL && vector->size == 0) {
        assert(false);
    }
    --(vector->size);
    return vector->data[vector->size];
}

static inline value_t vector_front(vector_t* vector) {
    assert(vector);
    assert(vector->size > 0);
    return vector->data[0];
}

static inline value_t vector_back(vector_t* vector) {
    assert(vector);
    assert(vector->data);
    assert(vector->size > 0);
    return vector->data[vector->size - 1];
}

#endif // _VECTOR_H_
