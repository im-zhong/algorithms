// 2022/11/18
// zhangzhong
// vector

#ifndef CONTAINER_VECTOR_H
#define CONTAINER_VECTOR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    void* data;
    size_t type_size;
    size_t size;
    size_t capacity;
} vector_t;

vector_t* make_vector(size_t capacity, size_t type_size);
vector_t* copy_vector(vector_t* vector);
void clear_vector(vector_t* vector);
void free_vector(vector_t* vector);

bool vector_empty(vector_t* vector);
size_t vector_size(vector_t* vector);
void* vector_at(vector_t* vector, size_t i);

void* vector_front(vector_t* vector);
void* vector_back(vector_t* vector);
void vector_push_back(vector_t* vector, void* element);
void vector_pop_back(vector_t* vector);

#endif // CONTAINER_VECTOR_H
