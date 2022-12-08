// 2022/12/4
// zhangzhong
// test string

#include "string/kmp.h"
#include "string/sort.h"
#include "util/c_random.h"
#include "util/c_string.h"
#include "util/test.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void test_calculate_prefix() {
  char *pattern = "ababaca";
  size_t prefix[7];
  calculate_prefix(pattern, strlen(pattern), prefix);
}

void test_kmp() {
  // case1
  const char *text = "abababacababaca";
  const char *pattern = "ababaca";
  kmp(pattern, strlen(pattern), text, strlen(text));
}

void test_lsd_sort() {
  // 我们需要一个随机字符串数组
  const size_t n = 16;
  size_t len = 7;
  // 生成随机字符串数组
  rand_t r = make_rand('a', 'z');

  char *str[n];
  for (size_t i = 0; i < n; i++) {
    str[i] = random_string(&r, len).data;
  }

  lsd_sort(str, n, len);
  check_string_array_order(str, n);

  // free str
  for (size_t i = 0; i < n; i++) {
    free(str[i]);
  }
}

void test_msd_sort() {
  const size_t n = 128;
  size_t len = 7;
  // 生成随机字符串数组
  rand_t r = make_rand('a', 'z');

  char *str[n];
  for (size_t i = 0; i < n; i++) {
    str[i] = random_string(&r, len).data;
  }

  msd_sort(str, n);
  check_string_array_order(str, n);

  // free str
  for (size_t i = 0; i < n; i++) {
    free(str[i]);
  }
}

int main(int argc, char *argv[]) {
  test_calculate_prefix();
  test_kmp();
  test_lsd_sort();
  test_msd_sort();
  return 0;
}
