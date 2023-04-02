// 2023/4/2
// zhangzhong
// vector

#include "container/vector.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

vector_t* make_vector(size_t capacity, size_t type_size) {
    vector_t* vector = malloc(sizeof(vector_t));
    assert(vector);
    vector->data = NULL;
    vector->type_size = type_size;
    vector->size = 0;
    vector->capacity = capacity;
    if (capacity > 0) {
        // 分配空间
        vector->data = malloc(capacity * type_size);
        assert(vector->data);
    }
    return vector;
}

void free_vector(vector_t* vector) {
    assert(vector);

    free(vector->data);
    vector->data = NULL;
    vector->type_size = 0;
    vector->size = 0;
    vector->capacity = 0;
}

inline void vector_clear(vector_t* vector) { vector->size = 0; }

void vector_realloc(vector_t* vector) {
    assert(vector);

    if (vector->capacity == 0) {
        vector->capacity = 4;
    }

    vector->capacity <<= 1U;
    vector->data = realloc(vector->data, vector->capacity * vector->type_size);
}

vector_t* copy_vector(vector_t* vector) {
    assert(vector);
    vector_t* copy = make_vector(vector->capacity, vector->type_size);
    copy->size = vector->size;
    copy->capacity = vector->capacity;
    // 复制数据
    if (copy->data && vector->data) {
        memcpy(copy->data, vector->data, vector->size * vector->type_size);
    }
    return copy;
}

void* vector_at(vector_t* vector, size_t i) {
    assert(vector);
    assert(i < vector->size);
    return vector->data + i * vector->type_size;
}

bool vector_empty(vector_t* vector) {
    assert(vector);
    return vector->size == 0;
}

size_t vector_size(vector_t* vector) {
    assert(vector);
    return vector->size;
}

static inline void* vector_at_capacity(vector_t* vector, size_t i) {
    assert(vector);
    assert(i < vector->capacity);
    return vector->data + i * vector->type_size;
}

// 非常难用
// 还不如就写一个的int的vector呢
// 虽然也没用
void vector_push_back(vector_t* vector, void* element) {
    assert(vector);

    if (vector->size + 1 > vector->capacity) {
        vector_realloc(vector);
    }

    // 这里的问题 realloc只会改变capacity
    // vector_at会考虑size
    // 在这里是不对的 但是用户只能用size
    memcpy(vector_at_capacity(vector, vector->size), element,
           vector->type_size);
    ++(vector->size);
}

void vector_pop_back(vector_t* vector) {
    assert(vector);
    if (vector->data == NULL && vector->size == 0) {
        assert(false);
    }
    --(vector->size);
}

void* vector_front(vector_t* vector) {
    assert(vector);
    assert(vector->size > 0);
    return vector_at(vector, 0);
}

void* vector_back(vector_t* vector) {
    assert(vector);
    assert(vector->data);
    assert(vector->size > 0);
    return vector_at(vector, vector->size - 1);
}
