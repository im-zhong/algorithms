// 2022/11/5
// zhangzhong

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// 如果想用这个数组同时表示指针
// 那么必须用64数据类型
typedef int64_t value_t;

// array是一个从小到大排列的数组
// 然后数组的元素一定是互不相同的
// 或者写成一个assert也是可以的
bool array_check_impl(value_t *array, size_t size);

void array_check(value_t *array, size_t size);

bool array_search(value_t *array, size_t size, value_t key, size_t *pos);

// 将数组指定位置的数据删除
void array_delete(value_t *array, size_t size, size_t pos);

// 写一个辅助函数 insert
// 我们时常需要将一个数插入一个有序数组中
// 并返回其位置
size_t array_insert(value_t *array, size_t n, value_t key);

void array_insert_array(value_t *dest, size_t ndest, value_t *src, size_t nsrc,
                        size_t pos);

#endif // _ARRAY_H_