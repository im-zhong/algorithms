// 2022/12/8
// zhangzhong
// string sort

// 字符串长度相等的地位优先排序
// 基于快排的字符串高位优先排序

#ifndef _STRING_SORT_H_
#define _STRING_SORT_H_

#include "stddef.h"

// 使用计数排序的地位优先排序
// least-significant digit sort
void lsd_sort(char **str, size_t n, size_t len);

// 使用快排的高位优先
// most significant digit sort
void msd_sort(char **str, int n);

#endif // _STRING_SORT_H_