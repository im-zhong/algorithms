// 2022/12/10
// zhangzhong
// bit buffer

#ifndef __BITBUF__
#define __BITBUF__

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define BITBUF_SIZE 2

// bitbuf读写不能同时使用
// 要么只能用读 要么只能用写
typedef struct {

  // buffer里面应该放什么
  // 我们还应该可以控制buf的大小
  // 但是不需要用户提供参数 尽可能让使用变得简单

  // 为什么要用character??
  // 因为我们从文件中读取内容是一次读一个byte吧
  char buf[BITBUF_SIZE];
  // 表示现在buf中的bit数量
  // 只有read过程会用到n
  size_t n;
  FILE *stream;

  // 那我们得有一个offset呀
  // 就像普通的文件读写那样 一个offset指明当前的位置
  // 只不过这个offset是以bit为单位的 而不是byte

  size_t offset;

  // 给出一个最大的offset??

} bitbuf_t;

bitbuf_t *make_bitbuf(FILE *stream);

void free_bitbuf(bitbuf_t *bf);

// 读出一个bit
// 模仿unix的read和write返回的方式
// bit的类型用int是不是更好一点, 这样编程可以用 write(0) write(1) 这样
// 而不是别扭的 write(false) write(true)
int bitbuf_read(bitbuf_t *bf, int *bit);

// 将一个bit写入bf
int bitbuf_write(bitbuf_t *bf, int bit);

int bitbuf_flush(bitbuf_t *bf);

#endif // __BITBUF__
