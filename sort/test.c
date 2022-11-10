// 2022/11/10
// zhangzhong
// test sort

#include "sort.h"
#include <assert.h>

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
  value_t nums2[] = {1, 2};
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

int main(int argc, char *argv[]) {
  test_sort(selection_sort);
  test_sort(insertion_sort);
  test_sort(shell_sort);
  test_sort(bubble_sort);
  test_sort(merge_sort);
  test_sort(quick_sort);
  return 0;
}