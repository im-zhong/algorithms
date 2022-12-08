// 2022/12/4
// zhangzhong
// a simple test framework

#include "util/test.h"
#include "util/c_random.h"
#include <assert.h>
#include <string.h>

void check_array_order(value_t *data, size_t size) {
  assert(data);
  if (size > 0) {
    for (size_t i = 0; i < size - 1; ++i) {
      assert(data[i] <= data[i + 1]);
    }
  }
}

string_t random_string(rand_t *r, size_t len) {
  string_t str = make_string("");
  for (size_t i = 0; i < len; i++) {
    string_append(&str, "%c", (char)crand(r));
  }
  return str;
}

void check_string_array_order(char **str, size_t n) {
  assert(str);
  for (size_t i = 1; i < n; i++) {
    assert(str[i - 1]);
    assert(str[i]);
    assert(strcmp(str[i - 1], str[i]) <= 0);
  }
}
