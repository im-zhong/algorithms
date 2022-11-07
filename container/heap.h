// 2022/11/7
// zhangzhong
// 重写二叉堆

#ifndef __REFACTOR_HEAP_H__
#define __REFACTOR_HEAP_H__

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 处于学习的目的 不提供类似模板的操作 数据类型同意是int类型
// 好调试 逻辑也简单 代码也少 只是学习 不是为了写一个通用的数据结构

typedef int64_t value_t;

// 底层使用变长数组实现
typedef struct {
  value_t *data;
  size_t size;
  size_t capacity;
} heap_t;

void heap_init(heap_t *heap, size_t capacity) {
  heap->size = 0;
  // 我需要找到距离size最近的一个 2^n
  heap->capacity = capacity;
  // 我需要分配一个数组
  heap->data = (value_t *)malloc(heap->capacity * sizeof(value_t));
}

void heap_clean(heap_t *heap) { heap->size = 0; }

void heap_free(heap_t *heap) { free(heap->data); }

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
// G / x = 7 / 2 = 3:       right_child / 2 = parent,
// 上面两条可以总结成 child / 2 = parent
// 当然也有一些特例，比如root的parent还是零；而叶子节点没有对应的孩子，直接计算其下标将溢出
// 不过这些情况都可以很简单的判断出来
//

// 这里不从零开始是为了简单
// 如果下标从零开始 那么下标之间就不满足这些性质了
// !!因为下标从1开始了 所以以往的越界检测 从 i < size 变成了 i <= size
// size/2 之前的都是内部节点 之后的都是叶子节点
static inline size_t heap_root() { return 1; }

static inline size_t heap_parent(size_t child) { return child >> 1; }

static inline size_t heap_left(size_t parent) { return parent << 1; }

static inline size_t heap_right(size_t parent) { return heap_left(parent) + 1; }

typedef bool (*cmp_fn)(value_t, value_t);

static void heap_fixdown(heap_t *heap, size_t parent, cmp_fn cmp) {
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
      child = cmp(heap->data[left], heap->data[right]) ? left : right;
    }

    // 然后比较parent和child
    // 如果child比parent大 那么交换两者
    // 然后继续向下
    if (cmp(heap->data[parent], heap->data[child])) {
      // 无事发生
      // 到这里函数就结束了呀
      return;
    } else {
      // 交换两者
      value_t temp = heap->data[parent];
      heap->data[parent] = heap->data[child];
      heap->data[child] = temp;
      // 用child替换parent继续向下修正堆
      parent = child;
    }
  }
}

static void heap_fixup(heap_t *heap, size_t child, cmp_fn cmp) {
  // 我们会不断的和parent去比 直到 parent, child 满足 cmp
  // 或者没有parent了
  size_t parent = 0;
  while ((parent = heap_parent(child)) != 0 &&
         !cmp(heap->data[parent], heap->data[child])) {
    // 交换parent和child
    value_t temp = heap->data[parent];
    heap->data[parent] = heap->data[child];
    heap->data[child] = temp;
    // 继续上修
    child = parent;
  }
}

static inline bool less_equal(value_t left, value_t right) {
  return left <= right;
}
static inline bool greater_equal(value_t left, value_t right) {
  return left >= right;
}

static void max_heapify(heap_t *heap, size_t parent) {
  heap_fixdown(heap, parent, greater_equal);
}

// min_heapfiy和max_hepify不能说特别相似 只能说一摸一样
// 就是比较函数从 >= 换成 <=
static void min_heapify(heap_t *heap, size_t parent) {
  heap_fixdown(heap, parent, less_equal);
}

value_t heap_top(heap_t *heap) { return heap->data[heap_root()]; }

// heap_pop
// 现在还有一个关键的问题，就是size应该如何表示
// 我们的size最好等于实际使用的数量，这样有助于capacity相关的reallocate的实现
// 其实也无所谓啦，关键看哪个实现起来更优雅
// 那就选择用size表示实际的元素数量，然后我们实际占用的数组长度是size + 1,
// 因为0未使用 数组下标是从 [1, size] 所以第一个元素是data[1],
// 而最后一个元素是data[size]
value_t max_heap_pop(heap_t *heap) {
  // 其实left一定是小于right，我们可以削减判断的次数

  // 这个函数最好可以和fixdown结合起来，思考一下
  // 我这个其实相当于
  // bheap_fixdown(heap, bheap_root())

  // 不对不对，我又理解错了
  // pop之后，我们需要将最后一个位置的元素放到一个正确的位置中
  // 那么什么是一个正确的位置呢
  // 其实就是把最后一个元素放到root的位置
  // 然后启动一个向下过滤的过程而已
  heap->data[heap_root()] = heap->data[heap->size];
  --heap->size;
  // 然后启动一次下滤
  max_heapify(heap, heap_root());
}

value_t min_heap_pop(heap_t *heap) {
  // 其实left一定是小于right，我们可以削减判断的次数

  // 这个函数最好可以和fixdown结合起来，思考一下
  // 我这个其实相当于
  // bheap_fixdown(heap, bheap_root())

  // 不对不对，我又理解错了
  // pop之后，我们需要将最后一个位置的元素放到一个正确的位置中
  // 那么什么是一个正确的位置呢
  // 其实就是把最后一个元素放到root的位置
  // 然后启动一个向下过滤的过程而已
  heap->data[heap->size] = heap->data[heap_root()];
  --heap->size;
  // 然后启动一次下滤
  min_heapify(heap, heap_root());
}

static void heap_insert(heap_t *heap, value_t key, cmp_fn cmp) {
  // 插入一个值非常的简单
  // 就是把值放到最后一个位置
  // 然后从最后一个位置执行一次上修
  // size不能超过capacity
  assert(heap->size < heap->capacity);
  heap->data[heap->size + 1] = key;
  ++heap->size;
  heap_fixup(heap, heap->size, cmp);
}

value_t max_heap_insert(heap_t *heap, value_t key) {
  heap_insert(heap, key, greater_equal);
}

value_t min_heap_insert(heap_t *heap, value_t key) {
  heap_insert(heap, key, less_equal);
}

static void make_heap(heap_t *heap, cmp_fn cmp) {
  for (size_t i = heap_parent(heap->size); i > heap_root(); --i) {
    heap_fixdown(heap, i, cmp);
  }
}

// 修改heap里面的数据
void max_heap_init(heap_t *heap) { make_heap(heap, greater_equal); }
void min_heap_init(heap_t *heap) { make_heap(heap, less_equal); }

// heap_sort 非常简单

void heap_check(heap_t *heap, cmp_fn cmp) {
  size_t child = 0;
  // 判断是否是一个二叉堆特别简单
  // 只需要遍历非叶子节点，然后判断一下他们的孩子是不是都比他们大或者小
  // 总之需要满足堆的性质
  // 而叶子节点是没有孩子的，所以是无需判断的，因为他们都是一个堆
  for (size_t i = heap_root(); i <= heap->size / 2; ++i) {
    size_t left = heap_left(i);
    if (left <= heap->size) {
      // 有左孩子
      assert(cmp(heap->data[i], heap->data[left]));
    }
    size_t right = heap_right(i);
    if (child <= heap->size) {
      assert(cmp(heap->data[i], heap->data[right]));
    }
  }
}

void max_heap_check(heap_t *heap) { heap_check(heap, greater_equal); }

void min_heap_check(heap_t *heap) { heap_check(heap, less_equal); }

#endif // __REFACTOR_HEAP_H__