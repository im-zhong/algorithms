// 2023/3/19
// zhangzhong
// run length encoding

#ifndef COMPRESS_RUN_LENGTH_ENCODING_H
#define COMPRESS_RUN_LENGTH_ENCODING_H

// 8 bit 0 - 255
#define RUN_LENGTH 255

int run_length_compress(const char* input, const char* output);
int run_length_decompress(const char* input, const char* output);

#endif // COMPRESS_RUN_LENGTH_ENCODING_H
