// 2023/3/20
// zhangzhong
// lzw compression and expansion

#ifndef COMPRESS_LZW_H
#define COMPRESS_LZW_H

int lzw_compression(const char* input, const char* output);
int lzw_expansion(const char* input, const char* output);

#endif // COMPRESS_LZW_H
