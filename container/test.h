// 2022/11/20
// zhangzhong
// 提供一些函数辅助测试

// 比如说生成随机数组 之类的
// 检查数组顺序之类的

#ifndef __TEST__
#define __TEST__

#include "def.h"
#include <assert.h>
#include <stddef.h>

void check_array_order(value_t *data, size_t size) {
  assert(data);
  if (size > 0) {
    for (size_t i = 0; i < size - 1; ++i) {
      assert(data[i] < data[i + 1]);
    }
  }
}

#endif // __TEST__