// 2022/11/11
// zhangzhong

// 我需要生成一系列在指定范围内部的随机数

#include "util/c_random.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// typedef struct {
//   time_t seed;
//   int min;
//   int max;
// } rand_t;

rand_t make_rand(int min, int max) {
  assert(max >= min);
  rand_t rand;
  rand.seed = time(NULL);
  rand.min = min;
  rand.max = max;

  // 这个说实话只应该被调用一次
  // 不过也并不重要
  srand(rand.seed);
  return rand;
}

int crand(rand_t *r) {
  int x = rand();
  // printf("random: %d\n", x);
  // 0 - RAND_AMX
  // low - high
  // [a, b] -> [c, d]
  // (x - a) / (b - a) = (y - c) / (d - c)
  // y = c + (d - c) * (x - a) / (b - a)
  // 这个式子是不对的呀，如果max -min 非常大 或者我们生成的随机数非常大
  // 这个式子是溢出的 溢出会导致r->min + 负数 导致assert
  // min -> max之间的可能并不是均匀分布的 i think
  // 不对 这个式子真的没法用
  // int y = r->min + (r->max - r->min) * (x - 0) / (RAND_MAX - 0);
  // if (y < r->min) {
  //   y = r->min;
  // }
  // if (y > r->max) {
  //   y = r->max;
  // }
  // if (!(y >= r->min && y <= r->max)) {
  //   printf("min: %d, max: %d, random: %d, y: %d\n", r->min, r->max, x, y);
  //   assert(false);
  // }

  // 这里还是可能溢出
  // 如果 r->max == RAND_MAX, r->min == 0
  // 这个式子还是溢出了
  int y = 0;
  if (r->max - r->min != RAND_MAX) {
    y = r->min + x % (r->max - r->min + 1);
  } else {
    y = x;
  }
  if (!(y >= r->min && y <= r->max)) {
    printf("min: %d, max: %d, random: %d, y: %d\n", r->min, r->max, x, y);
    assert(false);
  }
  return y;
}
