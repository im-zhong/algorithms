// 2022/11/20
// zhangzhong
// 提供一些函数辅助测试

// 比如说生成随机数组 之类的
// 检查数组顺序之类的

#ifndef __TEST__
#define __TEST__

#include "c_random.h"
#include "c_string.h"
#include <stddef.h>
#include <stdint.h>

typedef int64_t value_t;

void check_array_order(value_t *data, size_t size);

// 生成给定长度的字符串
// 字符串的范围是 a-z
string_t random_string(rand_t *r, size_t len);

// 字符串数组的最后一个元素应该是 空指针
void check_string_array_order(char **str, size_t n);

#endif // __TEST__
