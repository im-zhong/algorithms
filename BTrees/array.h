// 2022/11/5
// zhangzhong

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdbool.h>
#include <stddef.h>

// array是一个从小到大排列的数组
// 然后数组的元素一定是互不相同的
// 或者写成一个assert也是可以的
bool array_check_impl(int *array, size_t size);

void array_check(int *array, size_t size);

bool array_search(int *array, size_t size, int key, size_t *pos);

// 将数组指定位置的数据删除
void array_delete(int *array, size_t size, size_t pos);

// 写一个辅助函数 insert
// 我们时常需要将一个数插入一个有序数组中
// 并返回其位置
size_t array_insert(int *array, size_t n, int key);

// test
void test_array_search();
void test_array_delete();
void test_array_insert();

#endif // _ARRAY_H_