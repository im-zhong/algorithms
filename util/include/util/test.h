// 2022/11/20
// zhangzhong
// 提供一些函数辅助测试

// 比如说生成随机数组 之类的
// 检查数组顺序之类的

#ifndef __TEST__
#define __TEST__

#include <stddef.h>
#include <stdint.h>

typedef int64_t value_t;

void check_array_order(value_t *data, size_t size);

#endif // __TEST__
