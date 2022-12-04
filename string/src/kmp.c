// 2022/11/4
// zhangzhong
// kmp

#include "string/kmp.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// prefix的长度和pattern的长度需要一样
// 函数不会检查
void calculate_prefix(const char *pattern, size_t npattern, size_t *prefix) {
  if (pattern == NULL || npattern == 0 || prefix == NULL) {
    return;
  }

  size_t state = 0;
  size_t index = 0;
  // back path: state1 -> state0
  prefix[0] = 0;
  for (index = 1; index < npattern; index++) {
    while (state > 0 && pattern[state] != pattern[index]) {
      // back
      // 这里减1是因为数组下标从0开始
      // 数组下标与状态的表示差1的
      state = prefix[state - 1];
    }

    // state = 0, index = 1
    // 然后和自己匹配 然后index一直++
    // 直到碰到一样的 state++
    if (pattern[state] == pattern[index]) {
      state++;
    }

    // 写上back路径
    // back path: state_index+1 -> state
    prefix[index] = state;
  }
}

void kmp(const char *pattern, size_t npattern, const char *text, size_t ntext) {
  if (pattern == NULL || text == NULL || npattern == 0 || ntext == 0) {
    return;
  }

  size_t prefix[npattern];
  // 1 -> 0
  prefix[0] = 0;
  // calculate prefix
  calculate_prefix(pattern, npattern, prefix);

  size_t state = 0;
  for (size_t index = 0; index < ntext; index++) {
    // back path
    while (state > 0 && text[index] != pattern[state]) {
      state = prefix[state - 1];
    }

    // if current char match, state forward
    if (text[index] == pattern[state]) {
      state += 1;
    }

    if (state == npattern) {
      // match
      // back path
      printf("%lu\n", index - npattern + 1);
      state = prefix[state - 1];
    }
  }
}
