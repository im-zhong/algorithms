// 2023/3/19
// zhangzhong
// file utility

#ifndef UTIL_FILE_H
#define UTIL_FILE_H

#include <stdio.h>

FILE* open_file_for_read(const char* filename);
FILE* open_file_for_write(const char* filename);

// 生成一个随机文件, 长度为 size byte
// 成功的返回0
int generate_random_file(const char* filename, size_t size);

#endif // UTIL_FILE_H
