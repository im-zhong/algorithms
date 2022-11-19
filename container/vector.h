// 2022/11/18
// zhangzhong
// vector

#ifndef __VECTOR__
#define __VECTOR__

#include <stddef.h>
#include <stdint.h>

typedef int64_t value_t;

typedef struct __vector {
  value_t *data;
  size_t size;
  size_t capacity;
} vector_t;

vector_t make_vector(size_t capacity);

// 非常难用
// 还不如就写一个的int的vector呢
// 虽然也没用
void vector_push_back(vector_t &vector, value_t element);
void vector_pop_back(vector_t &vector);

value_t vector_front(vector_t &vector);
value_t vector_back(vector_t &vector);

void vector_free(vector &t vector);
void vector_clear(vector_t &vector);

#endif // __VECTOR_H__
