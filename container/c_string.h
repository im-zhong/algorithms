// 2022/11/11
// zhangzhong
// 实现一个类似C++的string

#ifndef __C_STRING_H__
#define __C_STRING_H__

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 为了与现有的c的str系列函数想符合
// 数据结构的设计必须一致
// 我可以保证这里保存的字符串一定以\0结尾 可以省很多事
typedef struct {
  // size不包含null byte 但是我们的capacity至少比size大1 因为我们会保存null byte
  char *data;
  size_t size;
  size_t capacity;
} string_t;

// 区分两种类型的函数
// 一种以c字符串，以\0结尾
// 另外一种需要给出首地址和长度
// 每一个函数都应该对上述两种类型给出接口

// 连接两个字符串
void string_concat(string_t *string, char *data, size_t size) {
  assert(string);
  // 但是string本身可能是空的
  if (string->data) {
    // 最小 capacity = string->size + size + 1
    if (string->size + size >= string->capacity) {
      // realloc
      string->capacity = (string->size + size) * 2;
      string->data = (char *)realloc(string->data, string->capacity);
    }
    // 然后将data复制到string之后
    strncpy(string->data + string->size, data, size);
    // 最后放置一个nullbyte
    string->size += size;
    string->data[string->size] = '\0';
  } else {
    // 否则string是空的 我们直接把data复制过去就行了
    string->data = data;
    string->size = size;
    string->capacity = size + 1;
  }
}

void string_free(string_t *string) {
  free(string->data);
  string->data = NULL;
  string->size = 0;
  string->capacity = 0;
}

void free_string(string_t *string) {
  free(string->data);
  string->data = NULL;
  string->size = 0;
  string->capacity = 0;
}

// 还有一系列的to_string 最好是包装一下
// 然后还得有一个append函数

// 感觉可以写成一个
void string_append(string_t *string, const char *fmt, ...) {
  assert(string);
  char *data = NULL;
  va_list ap;
  va_start(ap, fmt);
  // Upon successfult return, these function return the number of characters
  // printed, excluding the null byte to end output to strings
  size_t size = vasprintf(&data, fmt, ap);
  va_end(ap);
  string_concat(string, data, size);
}

string_t make_string(const char *fmt, ...) {
  string_t string;
  string.size = 0;
  string.data = NULL;
  string.capacity = 0;
  va_list ap;
  va_start(ap, fmt);
  // 这样写不对的，append的输入参数 ... 我们不能传ap
  // string_append(&string, fmt, ap);
  char *data = NULL;
  size_t size = vasprintf(&data, fmt, ap);
  va_end(ap);
  string_concat(&string, data, size);
  return string;
}

void string_clear(string_t *string) {
  assert(string);
  string->size = 0;
  if (string->capacity > 0) {
    string->data[0] = '\0';
  }
}

#endif // __C_STRING_H__
