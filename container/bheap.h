// 2021/9/16
// zhangzhong
// 二叉堆

// 因为stl和linux内核都是使用的最简单的数组实现的二叉堆
// 那我也就这么实现了
// 只不过这次linux内核没有采用像红黑树那样，需要用户自己实现关键的函数
// 反而是使用了函数指针，这两种选择其实就是一个trade-off
// 只不过在这里我需要自行实现一个可变数组
// 我可以看看linux内核有没有实现可变数组这个功能,,,并没有，，，那就真的只是一个定长的数组
// 而我肯定是要实现一个变长的数组的，就直接乘以2
// 用户在创建一个数据结构的时候，是可以不去指定一个初始长度的，那么我就来一个默认的长度2^n
// 然后用户插入的时候，就可以自行按照*2的倍率取分配新的空间就可以了

// 就仅仅实现一个二叉堆就可以了, 最基础的功能就可以

// 用户仍然希望在堆中存在一种类型 T
// 同时用户也必须提供一个可以比较T类型的比较函数
#ifndef __BHEAP_H__
#define __BHEAP_H__

#include "heap.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  // 这个是指向真正数据的指针
  void **data;
  size_t size;
  size_t capacity;

  // 用户在初始化一个堆的时候，同时传入一个该堆的比较函数
  // 因为我们的概念是一个堆，而不是最大堆或者最小堆
  // 我们的规则是 predicate(parent, child) is true
  // 这样我们就可以使用heap指针来调用这个函数了，而不用用户每次调用函数都需要传入一个指针，很拉跨
  bool (*predicate)(const void *lhs, const void *rhs);
} bheap_t;

void bheap_init(bheap_t *heap, size_t capacity,
                bool (*predicate)(const void *lhs, const void *rhs)) {
  heap->size = 0;
  heap->capacity = capacity;
  heap->predicate = predicate;
  // 我需要分配一个数组
  heap->data = (void **)malloc(heap->capacity * sizeof(void *));
}

void bheap_clear(bheap_t *heap) { heap->size = 0; }

void bheap_free(bheap_t *heap) { free(heap->data); }

// 二叉堆实际上是一个数组
// 用来表示一颗近似的完全二叉树
// 什么是完全二叉树呢？？
// 就是一颗被填满二叉树，如果把所有节点按照层先的顺序放到一个数组里面，每隔节点的孩子和父亲的下标是存在一个奇妙的关系的
//          A
//      B        C
//   D    E    F    G
//  H I  J K  L M  N O
//
//   A B C D E F G H I  J  K  L  M  N  O
// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
//
// 我们随便挑一对父子节点 C F G
// C 3, F 6, G 7
// C * 2 = F:               parent * 2 = left_child
// C * 2 + 1 = G:           parent * 2 + 1 = right_child
// F / 2 = 6 / 2 = 3 :      left_child / 2 = parent
// G / x = 7 / 2 = 3:       right_child / 2 = parent, 上面两条可以总结成 child /
// 2 = parent
// 当然也有一些特例，比如root的parent还是零；而叶子节点没有对应的孩子，直接计算其下标将溢出
// 不过这些情况都可以很简单的判断出来
//

static inline size_t bheap_root() { return 1; }

static inline size_t bheap_parent(size_t child) { return child >> 1; }

static inline size_t bheap_left(size_t parent) { return parent << 1; }

static inline size_t bheap_right(size_t parent) {
  return bheap_left(parent) + 1;
}

// 但是一个完全二叉树想要成为堆，还需要满族一些堆的性质
// 堆根据其需要满足的性质的不同，分成最大堆和最小堆
// 这两种堆实际是对称的，只需要实现其中一种即可
// 这里以最小堆为例
//
// 最小堆 min_heap
// 1. 除了根(index = 0)以外的所有节点i,均满足 heap[parent(i)] <= heap[i]
// 简单的说，就是除了根节点，所有节点的值都比其子节点要小; 同时根就是最小的节点
// 一句话就是，任意的节点小于他的后裔

// 需要支持的几个操作
// push pop top
// heapify: 在一次修改之后重新维护一个堆的性质？？
// make_heap: 构建一个堆

// void *heap_top(heap *h)
// {
//     return h->data + 1;
// }

static inline void *bheap_top(bheap_t *heap) { return heap->data[heap_root()]; }

static inline bool bheap_is_empty(bheap_t *heap) { return heap->size == 0; }

static inline bool bheap_is_full(bheap_t *heap) {
  return heap->size == heap->capacity;
}

size_t bheap_fixdown(bheap_t *heap, size_t parent) {
  size_t left = 0;
  size_t right = 0;
  size_t child = 0;
  while ((left = heap_left(parent)) <= heap->size) {
    // 我首先要在left和right中找出最小的哪一个
    // 如果只有左孩子，就只和左孩子比
    child = left;
    // 如果我们还有右孩子，就先比较两个孩子
    right = left + 1;
    if (right <= heap->size) {
      child =
          heap->predicate(heap->data[left], heap->data[right]) ? left : right;
    }

    // 然后比较parent和child
    // 如果child比parent大 那么交换两者
    // 然后继续向下
    if (heap->predicate(heap->data[parent], heap->data[child])) {
      // 无事发生
      // 到这里函数就结束了呀
      return parent;
    } else {
      // 交换两者
      void *temp = heap->data[parent];
      heap->data[parent] = heap->data[child];
      heap->data[child] = temp;
      // 用child替换parent继续向下修正堆
      parent = child;
    }
  }
  return parent;
}

// 我们把上滤和下滤这两个过程抽象出来
size_t bheap_fixup(bheap_t *heap, size_t child) {
  // 我们会不断的和parent去比 直到 parent, child 满足 cmp
  // 或者没有parent了
  size_t parent = 0;
  while ((parent = heap_parent(child)) != 0 &&
         !heap->predicate(heap->data[parent], heap->data[child])) {
    // 交换parent和child
    void *temp = heap->data[parent];
    heap->data[parent] = heap->data[child];
    heap->data[child] = temp;
    // 继续上修
    child = parent;
  }
  return child;
}

// 现在还有一个关键的问题，就是size应该如何表示
// 我们的size最好等于实际使用的数量，这样有助于capacity相关的reallocate的实现
// 其实也无所谓啦，关键看哪个实现起来更优雅
// 那就选择用size表示实际的元素数量，然后我们实际占用的数组长度是size + 1,
// 因为0未使用 数组下标是从 [1, size] 所以第一个元素是data[1],
// 而最后一个元素是data[size]
void bheap_pop(bheap_t *heap) {
  // 其实left一定是小于right，我们可以削减判断的次数

  // 这个函数最好可以和fixdown结合起来，思考一下
  // 我这个其实相当于
  // bheap_fixdown(heap, bheap_root())

  // 不对不对，我又理解错了
  // pop之后，我们需要将最后一个位置的元素放到一个正确的位置中
  // 那么什么是一个正确的位置呢
  // 其实就是把最后一个元素放到root的位置
  // 然后启动一个向下过滤的过程而已
  void *value = heap->data[heap_root()];
  heap->data[heap_root()] = heap->data[heap->size];
  --heap->size;
  // 然后启动一次下滤
  bheap_fixdown(heap, heap_root());
}

// 还有一个操作，删除某个位置，这tm不和删除最小元是一样的？？？
// 没什么好实现的

void bheap_check(bheap_t *heap) {
  size_t child = 0;
  // 判断是否是一个二叉堆特别简单
  // 只需要遍历非叶子节点，然后判断一下他们的孩子是不是都比他们大或者小
  // 总之需要满足堆的性质
  // 而叶子节点是没有孩子的，所以是无需判断的，因为他们都是一个堆
  for (size_t i = heap_root(); i <= heap->size / 2; ++i) {
    size_t left = heap_left(i);
    if (left <= heap->size) {
      // 有左孩子
      assert(heap->predicate(heap->data[i], heap->data[left]));
    }
    size_t right = heap_right(i);
    if (child <= heap->size) {
      assert(heap->predicate(heap->data[i], heap->data[right]));
    }
  }
}

// 提供push操作
// 还要考虑reallocate
size_t bheap_push(bheap_t *heap, void *data) {
  // 插入一个值非常的简单
  // 就是把值放到最后一个位置
  // 然后从最后一个位置执行一次上修
  // size不能超过capacity
  assert(!bheap_is_full(heap));
  heap->data[heap->size + 1] = data;
  ++heap->size;
  return bheap_fixup(heap, heap->size);
}

// 还需要实现一个操作 就是修改某个人的值
// 就是说 我修改了size位置的值
// 然后调用这个函数修正一下整个堆
size_t bheap_update(bheap_t *heap, size_t pos) {
  assert(pos >= 1 && pos <= heap->size);
  size_t parent = bheap_parent(pos);
  // 不对啊 这里还要注意一些边界条件
  if (parent > 0 && !heap->predicate(heap->data[parent], heap->data[pos])) {
    return bheap_fixup(heap, pos);
  } else {
    return bheap_fixdown(heap, pos);
  }
}

static void make_bheap(bheap_t *heap) {
  for (size_t i = heap_parent(heap->size); i >= heap_root(); --i) {
    bheap_fixdown(heap, i);
  }
}

#endif // __BHEAP_H__