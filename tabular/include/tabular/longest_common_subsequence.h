// 2023/3/13
// zhangzhong
// dynamic programming: longest common subsequence

#ifndef LONGEST_COMMON_SUBSEQUENCE
#define LONGEST_COMMON_SUBSEQUENCE

#include <stddef.h>

// X = ABCBDAB
// Y = BDCABA
// Z = lcs(X, Y) = BCBA

// 我们也需要返回一个字符串
// input: 都需要以\0结尾 这样我可以获得他们的长度
// 这样写测试代码也简单 lcs("abcbdab", "bdcaba")
// output: 我们希望用户提供一个buffer 我们会将结果写入
// 并返回写入的序列长度
size_t longest_common_subsequence(const char* x, const char* y);

#endif // LONGEST_COMMON_SUBSEQUENCE
