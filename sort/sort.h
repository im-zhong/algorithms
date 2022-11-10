// 2022/11/10
// zhangzhong
// selection sort

// 找到最小的元素 与第一个交换
// 然后找到第二小的元素 与第二个交换
// ...

#ifndef __SORT_H__
#define __SORT_H__

#include <assert.h>
#include <container/heap.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// 我们都是按照 int64_t类型的数据进行比较
// 咱们把这个类型叫做 value_t

// 我不能用这个 value_t 因为标准库已经用了
typedef int64_t value_t;

void swap(value_t *key, size_t i, size_t j) {
  value_t temp = key[i];
  key[i] = key[j];
  key[j] = temp;
}

void selection_sort(value_t *key, size_t size) {
  size_t min = 0;
  for (size_t i = 0; i < size; ++i) {
    min = i;

    for (size_t j = i + 1; j < size; ++j) {
      if (key[min] > key[j]) {
        min = j;
      }
    }
    // 经过一轮移植后 交换 i 和 min
    swap(key, i, min);
  }
}

// 就像气泡一样 将大的数据一趟一趟的排到最后面
void bubble_sort(value_t *key, size_t size) {
  for (size_t i = size; i > 0; --i) {
    for (size_t j = 1; j < i; ++j) {
      if (key[j - 1] > key[j]) {
        swap(key, j - 1, j);
      }
    }
  }
}

void insertion_sort(value_t *key, size_t size) {
  // i从1开始
  for (size_t i = 1; i < size; ++i) {
    // 每一轮都看成是i+1 长度的数组
    // key[i] 看成是要插入的元素
    // 我们只需要从头开始比较 或者是从尾部开始
    // 因为我们倾向于数组已经是有序的 所以从尾部开始比较是快的

    // 错误案例
    // for (size_t j = i - 1; j >= 0; --j) {
    //   // bug: 因为我的下标用的是size_t 类型 当j是零的时候 --j会变成uintmax
    //   // size_t >= 0 是一定的 这个比较是没有意义的
    //   if (key[j] > key[i]) {
    //     // 交换两者
    //     swap(key, i, j);
    //   }
    // }

    for (size_t j = i; j > 0; --j) {
      if (key[j - 1] > key[j]) {
        swap(key, j - 1, j);
      } else {
        // 说明两者满足less qeual的关系
        // 那么这轮比较就可以结束了
        break;
      }
    }
  }
}

void shell_sort(value_t *key, size_t size) {
  // 来自算法4 具体是为什么就不知道了
  // h sequence: 1, 4, 13, 40, 121, 364
  size_t h = 1;
  while (h < size / 3)
    h = 3 * h + 1;

  // h 从大到小 直到1 h=1 就是insertion sort
  while (h >= 1) {
    // 按照这样实现的算法和可视化网站的比较顺序不一样
    // 不过结果是一样的
    // 实际上在每一轮 h 中
    // 所有相距为h的的元素都会比较然后交换
    for (size_t i = h; i < size; ++i) {
      for (size_t j = i; j >= h; j -= h) {
        if (key[j - h] > key[j]) {
          swap(key, j - h, j);
        } else {
          break;
        }
      }
    }
    h = h / 3;
  }
}

// 将 [low, mid], [mid+1, high] 执行双路合并
void merge(value_t *key, size_t low, size_t mid, size_t high) {

  if (high <= low) {
    return;
  }

  size_t size = high - low + 1;
  // 首先复制 [low, high]
  value_t *copy = (value_t *)malloc(size * sizeof(value_t));
  for (size_t i = low; i <= high; ++i) {
    copy[i - low] = key[i];
  }

  // i 表示 copy数组的下标
  size_t i = 0;
  // j表示 [low, mid] 数组的下标
  size_t j = low;
  // k 表示 [mid + 1, high] 数组的下标
  size_t k = mid + 1;
  while (i < size && j <= mid && k <= high) {
    if (key[j] < key[k]) {
      copy[i] = key[j];
      ++j;
    } else {
      copy[i] = key[k];
      ++k;
    }
    ++i;
  }

  // j 和 k可能有剩下的 放在i后面即可
  while (j <= mid) {
    copy[i] = key[j];
    ++i;
    ++j;
  }

  while (k <= high) {
    copy[i] = key[k];
    ++i;
    ++k;
  }

  assert(i == size);

  // 最后把copy数组复制到key数组的[low, high]部分
  for (size_t i = 0; i < size; ++i) {
    key[low + i] = copy[i];
  }
}

void merge_sort_impl(value_t *key, size_t low, size_t high) {

  // 递归结束条件
  if (high <= low) {
    return;
  }

  size_t mid = low + (high - low) / 2;
  merge_sort_impl(key, low, mid);
  merge_sort_impl(key, mid + 1, high);
  merge(key, low, mid, high);
}

void merge_sort(value_t *key, size_t size) {
  // merge操作非常的简单 就是不断的递归 二路归并
  merge_sort_impl(key, 0, size - 1);
}

// 这个操作就是快速排序的每一趟递归的操作
// 随机选择一个值 使得所有小于该值的元素都在前面 所有大于该值的元素都在后面
// 然后返回该元素的下标
size_t partition(value_t *key, size_t low, size_t high) {
  if (high <= low) {
    return low;
  }

  // 我们选择第一个元素作为枢纽 pivot
  value_t pivot = key[low];

  // 也可以像算法4那样选择 i = low; j = high + 1
  // 但是在下面的循环体中需要先++i 再判断 具体见算法4中的实现
  // 这里的实现是等价的
  size_t i = low + 1;
  // 算法导论和算法4都选择将远离pivot的那边变成high+1
  // 为什么？ 为了对称 因为我们选择了pivot 那么i应该是从low+1开始的
  size_t j = high;
  // 两个指针同时向中间移动， 直到碰到一起
  // bug: case 2: 这里必须是 while(true)
  // 我们必须要让j是因为碰到了小于等于pivot的值才退出 否则最后的交换是错误的
  // 第二个测试用例给出了这样的情况 while (i < j)
  while (true) {
    // 首先从左边找到一个大于枢纽的元素
    // 只是这么写可能会导致越界 如果pivot就是最大的元素呢
    // 所以需要边界检测
    while (i < high && key[i] <= pivot)
      ++i;

    // 然后从右边找到一个小于枢纽的元素
    while (j > low && key[j] >= pivot)
      --j;

    if (i < j) {
      // 交换两者
      swap(key, i, j);
    } else {
      break;
    }
  }

  // 最后将枢纽放在i和j处
  // 最后一定是与j进行交换 因为我们的pivot在最前面
  // 算法导论中pivot是最后一个值 所以相应的最后应该与i交换
  swap(key, low, j);
  return j;
}

void quick_sort_impl(value_t *key, size_t low, size_t high) {
  // 递归结束条件
  if (high <= low) {
    return;
  }

  // 递归
  size_t p = partition(key, low, high);
  // bug: 如果数组应该按照小于的顺序排好了 那么返回的p的位置将会是0
  // 这里会把p - 1 = uintmax 放到high里面 如果我们的索引类型是int
  // 这不会有什么问题 但是使用size_t就必须面对这个问题
  // 为什么要使用size_t呢？？我感觉这好像是平白无故的给自己添加编程的复杂度
  // 只不过是因为C++使用了size_t但是说实话 这真的有意义吗??
  // 就是数据的范围大了一点 但是我从来都用不上大的哪一点数据
  // 但是编程的复杂度确实增加了 无符号数有太多的容易出现bug的地方
  // 我应该重新审视自己了 应该放弃对size_t的执着
  if (p == 0) {
    quick_sort_impl(key, low, 0);
  } else {
    quick_sort_impl(key, low, p - 1);
  }
  quick_sort_impl(key, p + 1, high);
}

void quick_sort(value_t *key, size_t size) {
  quick_sort_impl(key, 0, size - 1);
}

// 还有堆排序 就是有一个问题 heap要求输入数组的0下标是不能使用的
// 这是实现细节决定的 木的办法
// 所以我们还是自己分配一个数组吧 但是堆排序是in-place的
void heap_sort(value_t *key, size_t size) {

  // 为了测试简单 还是提供一致的接口模型吧
  value_t *data = (value_t *)malloc((size + 1) * sizeof(value_t));
  for (size_t i = 0; i < size; ++i) {
    data[i + 1] = key[i];
  }

  heap_t heap;
  heap.data = data;
  heap.size = size;
  heap.capacity = size + 1;

  // 构建最大堆
  make_max_heap(&heap);

  // for 头尾交换 fixdown
  while (heap.size > 1) {
    swap(data, 1, heap.size);
    --heap.size;
    max_fixdown(&heap, 1);
  }

  // 再拷贝回去
  for (size_t i = 0; i < size; ++i) {
    key[i] = data[i + 1];
  }
}

// 三种线性时间排序
// 这三个排序就不实现了 应用范围太窄
// 不过他们的原理都非常简单 参考算法导论即可
// counting sort
// radix sort
// 桶排序和哈希表非常非常的像
// bucket sort

#endif // __SELECTION_SORT_H__