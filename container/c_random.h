// 2022/11/11
// zhangzhong

// 我需要生成一系列在指定范围内部的随机数

#ifndef __C_RANDOM_H__
#define __C_RANDOM_H__

#include <assert.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  time_t seed;
  int min;
  int max;
} rand_t;

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
  // 0 - RAND_AMX
  // low - high
  // [a, b] -> [c, d]
  // (x - a) / (b - a) = (y - c) / (d - c)
  // y = c + (d - c) * (x - a) / (b - a)
  int y = r->min + (r->max - r->min) * (x - 0) / (RAND_MAX - 0);
  assert(y >= r->min && y <= r->max);
  return y;
}

#endif // __C_RANDOM_H__
