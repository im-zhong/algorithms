// 2022/12/8
// zhangzhong
// string sort

#include "string/sort.h"
#include "util/c_random.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_SIZE 256

// 先写一个简单的计数排序
// 以字符串的下标i处作为索引 进行计数排序
static void count_sort(char **str, size_t n, size_t pos) {
  assert(pos < n);
  char **tmp = (char **)malloc(sizeof(char *) * n);

  // 字符的数量是256
  size_t count[ASCII_SIZE];
  for (size_t i = 0; i < ASCII_SIZE; i++) {
    count[i] = 0;
  }

  // 1. 统计索引
  for (size_t i = 0; i < n; i++) {
    count[str[i][pos]]++;
  }

  // 2. 累加
  for (size_t i = 1; i < ASCII_SIZE; i++) {
    count[i] += count[i - 1];
  }

  // 3. 排序
  for (size_t i = n; i > 0; i--) {
    tmp[count[str[i - 1][pos]] - 1] = str[i - 1];
    count[str[i - 1][pos]]--;
  }

  // 4. 写回
  for (size_t i = 0; i < n; i++) {
    str[i] = tmp[i];
  }
}

void lsd_sort(char **str, size_t n, size_t len) {
  for (size_t pos = len; pos > 0; pos--) {
    count_sort(str, n, pos - 1);
  }
}

static void swap(char **str, int i, int j) {
  char *tmp = str[i];
  str[i] = str[j];
  str[j] = tmp;
}

static void insertion_sort(char **str, int low, int high) {
  for (int i = low + 1; i <= high; i++) {
    for (int j = i; j > low; j--) {
      if (strcmp(str[j - 1], str[j]) > 0) {
        swap(str, j - 1, j);
      } else {
        break;
      }
    }
  }
}

static int random_middle(char **str, int low, int high) {
  // 随机取三个数
  // 这里的随机会导致问题
  // 用新的随机数的实现
  // rand_t r = make_rand(low, high);
  // int i = crand(&r);
  // int j = crand(&r);
  // int k = crand(&r);
  int i = rrand(low, high);
  int j = rrand(low, high);
  int k = rrand(low, high);

  // 取三个的中位数
  // 类似插入排序的方式比较他们
  if (strcmp(str[j], str[i]) < 0) {
    // i < j
    // 那我们还要看i和k的关系
    // 交换i和j
    int tmp = i;
    i = j;
    j = tmp;
  }

  // 现在i j就是有序的
  // 然后比较
  if (strcmp(str[k], str[j]) < 0) {
    // k < j
    // 交换
    int tmp = k;
    k = j;
    j = tmp;

    if (strcmp(str[j], str[i]) < 0) {
      int tmp = i;
      i = j;
      j = tmp;
    }
  }

  assert(strcmp(str[i], str[j]) <= 0);
  assert(strcmp(str[j], str[k]) <= 0);
  return j;
}

static int partition(char **str, int low, int high) {
  // 随机三数取中
  // 交换mid和low
  int mid = random_middle(str, low, high);
  swap(str, low, mid);

  // 然后执行正常的划分
  char *pivot = str[low];
  int i = low + 1;
  int j = high;
  while (true) {
    while (i < high && strcmp(str[i], pivot) <= 0)
      i++;

    while (j > low && strcmp(str[j], pivot) >= 0)
      j--;

    if (i < j) {
      swap(str, i, j);
    } else {
      break;
    }
  }

  swap(str, low, j);
  return j;
}

static void quick_sort_impl(char **str, int low, int high) {
  // 实现两个额外的优化
  // 1. 小数组进行插入排序
  // 2. 随机取三数取中位数
  if (high <= low) {
    return;
  }
  if (high - low < 16) {
    insertion_sort(str, low, high);
    return;
  }

  int p = partition(str, low, high);
  quick_sort_impl(str, low, p - 1);
  quick_sort_impl(str, p + 1, high);
}

void msd_sort(char **str, int n) { quick_sort_impl(str, 0, n - 1); }
