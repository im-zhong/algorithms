// 2022/11/11
// zhangzhong

// 我需要生成一系列在指定范围内部的随机数

#ifndef __C_RANDOM_H__
#define __C_RANDOM_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  time_t seed;
  int min;
  int max;
} rand_t;

rand_t make_rand(int min, int max);

int crand(rand_t *r);

#endif // __C_RANDOM_H__
