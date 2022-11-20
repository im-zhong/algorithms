// 2022/11/18
// zhangzhong
// vector

#ifndef __VECTOR__
#define __VECTOR__

#include "def.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct __vector {
  value_t *data;
  size_t size;
  size_t capacity;
} vector_t;

vector_t make_vector(size_t capacity) {
  vector_t vector;
  vector.data = NULL;
  vector.size = 0;
  vector.capacity = capacity;
  if (capacity > 0) {
    // 分配空间
    vector.data = (value_t *)malloc(capacity * sizeof(value_t));
    assert(vector.data);
  }
  return vector;
}

void realloc_vector(vector_t *vector) {
  assert(vector);

  if (vector->capacity == 0)
    vector->capacity = 4;

  vector->capacity <<= 1;
  vector->data =
      (value_t *)realloc(vector->data, vector->capacity * sizeof(value_t));
}

void free_vector(vector_t *vector) {
  assert(vector);

  free(vector->data);
  vector->data = NULL;
  vector->size = 0;
  vector->capacity = 0;
}

vector_t copy_vector(vector_t *vector) {
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
void vector_push_back(vector_t *vector, value_t element) {
  assert(vector);

  if (vector->size + 1 > vector->capacity)
    realloc_vector(vector);

  (vector->data)[vector->size] = element;
  ++(vector->size);
}

void vector_pop_back(vector_t *vector) {
  assert(vector);
  if (vector->data == NULL && vector->size == 0)
    assert(false);
  --(vector->size);
}

value_t vector_front(vector_t *vector) {
  assert(vector);
  assert(vector->size > 0);
  return vector->data[0];
}

value_t vector_back(vector_t *vector) {
  assert(vector);
  assert(vector->data);
  assert(vector->size > 0);
  return vector->data[vector->size - 1];
}

#endif // __VECTOR_H__
