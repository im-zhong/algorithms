// 2022/11/10
// zhangzhong
// selection sort

// 找到最小的元素 与第一个交换
// 然后找到第二小的元素 与第二个交换
// ...

#ifndef __SORT_H__
#define __SORT_H__

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// 我们都是按照 int64_t类型的数据进行比较
// 咱们把这个类型叫做 value_t

// 我不能用这个 value_t 因为标准库已经用了
typedef int64_t value_t;

void selection_sort(value_t *key, size_t size);

// 就像气泡一样 将大的数据一趟一趟的排到最后面
void bubble_sort(value_t *key, size_t size);

void insertion_sort(value_t *key, size_t size);

void shell_sort(value_t *key, size_t size);
// 将 [low, mid], [mid+1, high] 执行双路合并

void merge_sort(value_t *key, size_t size);

// 这个操作就是快速排序的每一趟递归的操作
// 随机选择一个值 使得所有小于该值的元素都在前面 所有大于该值的元素都在后面
// 然后返回该元素的下标

void quick_sort(value_t *key, size_t size);

// 还有堆排序 就是有一个问题 heap要求输入数组的0下标是不能使用的
// 这是实现细节决定的 木的办法
// 所以我们还是自己分配一个数组吧 但是堆排序是in-place的
void heap_sort(value_t *key, size_t size);
// 三种线性时间排序
// 这三个排序就不实现了 应用范围太窄
// 不过他们的原理都非常简单 参考算法导论即可
// counting sort
// radix sort
// 桶排序和哈希表非常非常的像
// bucket sort

#endif // __SELECTION_SORT_H__
