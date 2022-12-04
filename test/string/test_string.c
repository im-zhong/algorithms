// 2022/12/4
// zhangzhong
// test string

#include "string/kmp.h"
#include <stddef.h>
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

int main(int argc, char *argv[]) {
  test_calculate_prefix();
  test_kmp();
  return 0;
}
