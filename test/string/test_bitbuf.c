// 2022/12/10
// zhangzhong
// test bit buffer

#include "string/bitbuf.h"
#include "util/c_random.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// 怎么测呢?? 准备一个随机数组
#define NUM (4096)
// 不对 不对 我想起来了
// 这个数组就保存0或者1
int bits[NUM];
// 生成随机数即可

void test_bitbuf_write(char *filename) {
  // 先测试写 再测试读
  // 生成很多个随机的bit
  // 写进去再读出来 要求一摸一样

  // 生成随机bit
  for (int i = 0; i < NUM; i++) {
    bits[i] = rrand(0, 1);
  }

  FILE *stream = fopen(filename, "wb");
  if (stream == NULL) {
    printf("open %s failed.\n", filename);
    exit(EXIT_FAILURE);
  }

  bitbuf_t *bf = make_bitbuf(stream);
  for (int i = 0; i < NUM; i++) {
    bitbuf_write_bit(bf, bits[i]);
  }
  bitbuf_flush(bf);

  free_bitbuf(bf);
  fclose(stream);
}

void test_bitbuf_read(char *filename) {
  FILE *stream = fopen(filename, "rb");
  if (stream == NULL) {
    printf("open %s failed.\n", filename);
    exit(EXIT_FAILURE);
  }

  // 读取文件并验证是否一致
  bitbuf_t *bf = make_bitbuf(stream);
  int bit = 0;
  for (int i = 0; i < NUM; i++) {
    assert(bitbuf_read_bit(bf, &bit));
    assert(bit == bits[i]);
  }
  printf("\n");

  free_bitbuf(bf);
  fclose(stream);
}

int main(int argc, char *argv[]) {

  // 准备随机数组
  char *filename = "test_bitbuf.bin";
  test_bitbuf_write(filename);
  test_bitbuf_read(filename);
}
