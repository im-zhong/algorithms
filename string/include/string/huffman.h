// 2022/12/9
// zhangzhong
// huffman code

#ifndef __HUFFMAN__
#define __HUFFMAN__

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// 霍夫曼编码
// 分为五个阶段
// 读取输入字符
// 第一趟 1. 统计每个字符的出现频率
//       2. 通过频率构建一个哈夫曼树
//       3. 通过哈夫曼树 构建索引表
//       4. 在编码之前，先通过前序遍历将哈夫曼树输出，以供解码使用
// 第二趟 5. 再次读取文件，通过索引表转换为前缀编码输出

// 哈夫曼解码
// 1. 读入哈夫曼树
// 2. 读入编码流，从根节点开始根据0->left
//    1->right的规则，碰到叶子就输出字符，然后返回根节点重新开始这个过程

typedef struct huffman_node_t {
  struct huffman_node_t *left;
  struct huffman_node_t *right;
  size_t freq;
  int c;
} huffman_node_t;

// 提供两个接口 一个编码 一个解码
void huffman_compress(FILE *stream);

void huffman_decompress(FILE *stream);

#endif // __HUFFMAN__
