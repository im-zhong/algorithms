// 2022/11/11
// zhangzhong
// 实现一个类似C++的string

#ifndef __C_STRING_H__
#define __C_STRING_H__

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 为了与现有的c的str系列函数想符合
// 数据结构的设计必须一致
// 我可以保证这里保存的字符串一定以\0结尾 可以省很多事
typedef struct {
    // size不包含null byte 但是我们的capacity至少比size大1 因为我们会保存null
    // byte
    char* data;
    size_t size;
    size_t capacity;
} string_t;

// 区分两种类型的函数
// 一种以c字符串，以\0结尾
// 另外一种需要给出首地址和长度
// 每一个函数都应该对上述两种类型给出接口

// 连接两个字符串
void string_concat(string_t* string, char* data, size_t size);

void string_free(string_t* string);

void free_string(string_t* string);

// 还有一系列的to_string 最好是包装一下
// 然后还得有一个append函数

// 感觉可以写成一个
void string_append(string_t* string, const char* fmt, ...);

string_t make_string(const char* fmt, ...);

void string_clear(string_t* string);

// 实现两个函数用于支持trie
void string_pushback(string_t* string, char c);

int string_popback(string_t* string);

// 检查提供的字符串是不是当前字符串的后缀
bool string_is_suffix(string_t* string, char* suffix);

#endif // __C_STRING_H__
