// 2022/11/4
// zhangzhong
// 32.1 The native string-matching algorithm

#include <stddef.h>
#include <string.h>
#include <stdio.h>

// O((ntext - npattern + 1)*npattern)
// 总共比较 ntext - npattern + 1次，每次比较都是遍历 pattern 字符串
void native_strmatch(const char* text, size_t ntext, const char* pattern, size_t npattern) {
    // 要求就是 npattern <= ntext
    for (int i = 0; i < ntext - npattern + 1; ++i) {
        if (strncmp(text + i, pattern, npattern) == 0) {
            // match
            // 为了测试 我们可以输出match的位置
            // 一行一个 有利于测试
            printf("%d\n", i);
        }
    }
}
