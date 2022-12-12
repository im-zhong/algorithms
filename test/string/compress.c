// 2022/12/9
// zhangzhong
// compress and decompress using huffman code

#include "string/huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // 用户可以根据一个参数指定是压缩还是解码
  // 或者用更简单的方法
  // huf -c <input>
  // 这条命令会生成 input.huf 文件
  // huf -d <input>.huf
  // 这条命令会解压缩input.huf 生成 Input 文件
  if (argc != 3) {
    printf("Usage: %s -c|-d <filename>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "-c") == 0) {
    // 压缩
    // 不论哪种方式 我们都需要打开文件
    // FILE *stream = fopen(argv[2], "r");
    // if (stream == NULL) {
    //   printf("%s: open %s failed.\n", argv[0], argv[2]);
    //   exit(EXIT_FAILURE);
    // }

    // 我们需要顺序遍历一遍文件
    // 这其实是一个东西
    // 我们用两个参数传递了一个东西
    // 其实就是一个文件名
    huffman_compress(argv[2]);

    // fclose(stream);

  } else if (strcmp(argv[1], "-d") == 0) {
    // 解压缩
    huffman_decompress(argv[2]);

  } else {
    printf("Usage: %s -c|-d <filename>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}