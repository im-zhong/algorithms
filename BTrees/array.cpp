// 2022/11/5
// zhangzhong
// array.c

#include "array.h"
#include <assert.h>
#include <stdio.h>

bool array_check_impl(int *array, size_t size) {
  for (size_t i = 0; i < size - 1; ++i) {
    if (array[i] >= array[i + 1]) {
      return false;
    }
  }
  return true;
}

void array_check(int *array, size_t size) {
  assert(array_check_impl(array, size));
}

// 如果找到了，返回对应的下标
// 如果没有找到，应该返回夹住这个值的两个下标
// 但是返回哪个呢??
// 如果，我们说返回前一个，那么如果我们的值是最小值呢
// 如果我们返回后面一个，那么是可以的
// 所以我们返回后面那个值 [0, size]
bool array_search(int *array, size_t size, int key, size_t *pos) {
  for (size_t i = 0; i < size; ++i) {
    if (key <= array[i]) {
      *pos = i;
      return key == array[i];
    }
  }
  *pos = size;
  return false;
}

void array_delete(int *array, size_t size, size_t pos) {
  // 删掉这个位置的元素其实就是让 [pos+1, size-1]
  // 出的数据向前移动
  for (size_t i = pos + 1; i < size; ++i) {
    array[i - 1] = array[i];
  }
}

// 这个函数有一个要求，就是数组的 capacity 必须是足够的
// 因为向数组中插入数据会导致数组的大小超过size
size_t array_insert(int *array, size_t size, int key) {
  size_t pos = 0;
  if (!array_search(array, size, key, &pos)) {
    for (size_t i = size; i > pos; --i) {
      array[i] = array[i - 1];
    }
    array[pos] = key;
  }
  return pos;
}

void test_array_search() {
  int array[7] = {1, 2, 3, 4, 5, 6, 7};
  array_check(array, 7);

  // want: r = true, pos = 0
  size_t pos = 0;
  bool r = array_search(array, 7, 1, &pos);
  if (!(r == true && pos == 0)) {
    printf("%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
           __FILE__, __LINE__, __func__, true, 0lu, r, pos);
  }

  // want: r = true, pos = 6
  pos = 0;
  r = array_search(array, 7, 7, &pos);
  if (!(r == true && pos == 6)) {
    printf("%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
           __FILE__, __LINE__, __func__, true, 6lu, r, pos);
  }

  // want: r = false, pos = 0
  pos = 0;
  r = array_search(array, 7, -1, &pos);
  if (!(r == false && pos == 0)) {
    printf("%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
           __FILE__, __LINE__, __func__, false, 0lu, r, pos);
  }

  // want: r = false, pos = 7
  pos = 0;
  r = array_search(array, 7, 8, &pos);
  if (!(r == false && pos == 7)) {
    printf("%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
           __FILE__, __LINE__, __func__, false, 7lu, r, pos);
  }
}

bool array_equal(int *array1, size_t narray1, int *array2, size_t narray2) {
  if (narray1 != narray2) {
    return false;
  }
  for (size_t i = 0; i < narray1; ++i) {
    if (array1[i] != array2[i]) {
      return false;
    }
  }
  return true;
}

void test_array_delete() {
  // want r = true
  int array[8] = {1, 2, 3, 4, 5, 6, 7};
  int array1[6] = {2, 3, 4, 5, 6, 7};
  // delete 1
  array_delete(array, 7, 0);
  bool r = array_equal(array, 6, array1, 6);
  if (!(r == true)) {
    printf("%s:%d %s failed. want r = %d, but r = %d.\n", __FILE__, __LINE__,
           __func__, true, r);
  }

  // want r = true
  // array = {2, 3, 4, 5, 6, 7}
  int array2[5] = {2, 3, 4, 5, 6};
  // delete 7
  array_delete(array, 6, 5);
  r = array_equal(array, 5, array2, 5);
  if (!(r == true)) {
    printf("%s:%d %s failed. want r = %d, but r = %d.\n", __FILE__, __LINE__,
           __func__, true, r);
  }

  // want r = true
  // array = {2, 3, 4, 5, 6}
  int array3[5] = {2, 3, 5, 6};
  // delete 4
  array_delete(array, 5, 2);
  r = array_equal(array, 4, array3, 4);
  if (!(r == true)) {
    printf("%s:%d %s failed. want r = %d, but r = %d.\n", __FILE__, __LINE__,
           __func__, true, r);
  }
}

void test_array_insert() {
  // case 1: insert 1
  int array[8] = {2, 3, 5, 6};
  int array1[] = {1, 2, 3, 5, 6};
  size_t pos = array_insert(array, 4, 1);
  bool r = array_equal(array, 5, array1, 5);
  if (!(r == true && pos == 0)) {
    printf("%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
           __FILE__, __LINE__, __func__, true, 0lu, r, pos);
  }

  // case 2: insert 4
  // array[] = {1, 2, 3, 5, 6}
  int array2[] = {1, 2, 3, 4, 5, 6};
  pos = array_insert(array, 5, 4);
  r = array_equal(array, 6, array2, 6);
  if (!(r == true && pos == 3)) {
    printf("%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
           __FILE__, __LINE__, __func__, true, 3lu, r, pos);
  }

  // case 3: insert 7
  // array[] = {1, 2, 3, 4, 5, 6}
  int array3[] = {1, 2, 3, 4, 5, 6, 7};
  pos = array_insert(array, 6, 7);
  r = array_equal(array, 7, array3, 7);
  if (!(r == true && pos == 6)) {
    printf("%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
           __FILE__, __LINE__, __func__, true, 6lu, r, pos);
  }
}
