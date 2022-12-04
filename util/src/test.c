// 2022/12/4
// zhangzhong
// a simple test framework

#include "util/test.h"
#include <assert.h>

void check_array_order(value_t *data, size_t size) {
  assert(data);
  if (size > 0) {
    for (size_t i = 0; i < size - 1; ++i) {
      assert(data[i] < data[i + 1]);
    }
  }
}
