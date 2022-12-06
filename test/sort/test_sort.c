// 2022/11/10
// zhangzhong
// test sort

#include "sort/sort.h"
#include "util/c_random.h"
#include "util/test.h"
#include <assert.h>
#include <stdio.h>

typedef void (*sort_fn)(value_t *, size_t);

void test_sort(sort_fn sort) {
  // case 1
  value_t nums1[] = {12, 32,  53,  56,  35, 35, 673, 2,  35,
                     46, 456, 235, 356, 2,  32, 42,  35, 345};
  size_t size = sizeof(nums1) / sizeof(value_t);
  sort(nums1, size);
  for (size_t i = 0; i < size - 1; ++i) {
    assert(nums1[i] <= nums1[i + 1]);
  }

  // case 2
  value_t nums2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  size_t size2 = sizeof(nums2) / sizeof(value_t);
  sort(nums2, size2);
  for (size_t i = 0; i < size2 - 1; ++i) {
    assert(nums2[i] <= nums2[i + 1]);
  }

  // case 3
  value_t nums3[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  size_t size3 = sizeof(nums3) / sizeof(value_t);
  sort(nums3, size3);
  for (size_t i = 0; i < size3 - 1; ++i) {
    assert(nums3[i] <= nums3[i + 1]);
  }
}

void test_select() {
  value_t nums1[] = {12, 32,  53,  56,  35, 35, 673, 2,  35,
                     46, 456, 235, 356, 2,  32, 42,  35, 345};
  size_t size = sizeof(nums1) / sizeof(value_t);
  quick_sort(nums1, size);

  value_t nums2[] = {12, 32,  53,  56,  35, 35, 673, 2,  35,
                     46, 456, 235, 356, 2,  32, 42,  35, 345};

  for (size_t i = 0; i < size; ++i) {
    value_t value = select_value(nums2, size, i);
    assert(value == nums1[i]);
    printf("%lu: %ld\n", i, value);
  }
}

void test_counting_sort() {
  // 随机生成序列就好
  const size_t size = 128;
  value_t nums[size];
  const value_t max = 256;

  rand_t rand = make_rand(0, max - 1);

  for (size_t i = 0; i < size; ++i) {
    nums[i] = crand(&rand);
  }

  // 然后就进行一个序的排
  counting_sort(nums, size, max);
  // 然后check array order
  check_array_order(nums, size);
}

void test_bucket_sort() {
  // 随机生成序列就好
  const size_t size = 16;
  value_t nums[size];
  const value_t max = 16;

  rand_t rand = make_rand(0, max - 1);

  for (size_t i = 0; i < size; ++i) {
    nums[i] = crand(&rand);
  }

  // 然后就进行一个序的排
  bucket_sort(nums, size, max);
  // 然后check array order
  check_array_order(nums, size);
}

int main(int argc, char *argv[]) {
  test_sort(selection_sort);
  test_sort(insertion_sort);
  test_sort(shell_sort);
  test_sort(bubble_sort);
  test_sort(merge_sort);
  test_sort(quick_sort);
  test_sort(heap_sort);
  // test select
  // 我怎么这么牛逼
  test_select();
  test_counting_sort();
  test_bucket_sort();
  return 0;
}