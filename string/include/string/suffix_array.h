// 2022/12/8
// zhangzhong
// suffix array

#ifndef __SUFFIX_ARRAY__
#define __SUFFIX_ARRAY__

// suffix array是一种数据结构
// 给定一个输入字符串 可以建立这种结构
// 我们对这种结构可以施加一些操作

// 在构造函数中提供一个字符串
// 我们就可以构造一个字符数组

#include "util/c_string.h"

// 简单改造一下
// 在读取文本的时候 记录line和col
typedef struct {
  char *str;
  int line;
  int col;
} suffix_t;

typedef struct {

  // 构造函数中提供的字符串
  // caution: 我们不会拷贝输入字符串
  string_t str;
  suffix_t *suffix;
  // 文本的长度 同时也是后缀数组的长度
  size_t n;
} suffix_array_t;

suffix_array_t *make_suffix_array(char *str, size_t n);

void free_suffix_array(suffix_array_t *sa);

// 在后缀数组上二分查找
int suffix_array_search(suffix_array_t *sa, char *str);

// 两个字符串的最长公共子前缀
// longest common prefix
// 返回最长公共子前缀的长度 比如 没有，就返回零
// abc abab 就返回2
int lcp(char *str1, char *str2);

// lrs的暴力解法 用于测试lrs的正确性
string_t lrs_slow(char *str);

// 最长重复子序列
// longest repeated substring
string_t lrs(char *str);

// 根据输入的文本构建一个后缀数组 在此后缀数组上支持快速的搜索
// keyword in context
void kwic(suffix_array_t *sa, char *str);

#endif // __SUFFIX_ARRAY__
