// 2022/11/5
// zhangzhong
// array.c

#include "array.h"
#include <assert.h>
#include <stdio.h>

bool array_check_impl(value_t *array, size_t size) {
  for (size_t i = 0; i < size - 1; ++i) {
    if (array[i] >= array[i + 1]) {
      return false;
    }
  }
  return true;
}

void array_check(value_t *array, size_t size) {
  assert(array_check_impl(array, size));
}

// 如果找到了，返回对应的下标
// 如果没有找到，应该返回夹住这个值的两个下标
// 但是返回哪个呢??
// 如果，我们说返回前一个，那么如果我们的值是最小值呢
// 如果我们返回后面一个，那么是可以的
// 所以我们返回后面那个值 [0, size]
bool array_search(value_t *array, size_t size, value_t key, size_t *pos) {
  for (size_t i = 0; i < size; ++i) {
    if (key <= array[i]) {
      *pos = i;
      return key == array[i];
    }
  }
  *pos = size;
  return false;
}

void array_delete(value_t *array, size_t size, size_t pos) {
  // 删掉这个位置的元素其实就是让 [pos+1, size-1]
  // 出的数据向前移动
  for (size_t i = pos + 1; i < size; ++i) {
    array[i - 1] = array[i];
  }
}

// 这个函数有一个要求，就是数组的 capacity 必须是足够的
// 因为向数组中插入数据会导致数组的大小超过size
size_t array_insert(value_t *array, size_t size, value_t key) {
  size_t pos = 0;
  if (!array_search(array, size, key, &pos)) {
    for (size_t i = size; i > pos; --i) {
      array[i] = array[i - 1];
    }
    array[pos] = key;
  }
  return pos;
}

// 将src插入dest的pos中
void array_insert_array(value_t *dest, size_t ndest, value_t *src, size_t nsrc,
                        size_t pos) {
  for (size_t i = ndest + nsrc - 1; i >= pos + nsrc; --i) {
    dest[i] = dest[i - nsrc];
  }
  for (size_t i = 0; i < nsrc; ++i) {
    dest[pos + i] = src[i];
  }
}
