// 2022/11/8
// zhangzhong
// 索引堆
// 来自算法4 不得不说这本书是写给人看的 算法导论不是写给人看的
// 跟普通的二项堆不同 他提供一个额外的索引功能
// 实现prim算法时非常有效

#ifndef __INDEX_HEAP_H__
#define __INDEX_HEAP_H__

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// index heap
typedef struct {
  // 堆的元素是放在外面的 必须是一个数组
  // 我们可以使用这个数组索引来访问堆的元素
  // 同时还满足对的特性
  void *value;
  size_t nvalue;
  size_t type_size;
  size_t *index;

  // 最大可以达到capacity
  size_t *key;
  size_t size;
  size_t capacity;
  bool (*predicate)(const void *lhs, const void *rhs);
} iheap_t;

// 他的接口和heap差不多
static inline void iheap_init(iheap_t *heap, void *value, size_t nvalue,
                              size_t type_size, size_t capacity,
                              bool (*predicate)(const void *lhs,
                                                const void *rhs));

static inline void iheap_clear(iheap_t *heap);
static inline void iheap_free(iheap_t *heap);

// 其实这些东西是通用的呀 可以写一个共同的头文件 放在里面
static inline size_t iheap_root() { return 1; }

static inline size_t iheap_parent(size_t child) { return child >> 1; }

static inline size_t iheap_left(size_t parent) { return parent << 1; }

static inline size_t iheap_right(size_t parent) {
  return iheap_left(parent) + 1;
}

// 因为 prim算法用不到make_heap 所以不实现了
// void make_iheap(iheap_t *heap);

// 与bheap不同的是 堆的元素由外部提供
// 并且保存在外部 所以我们返回索引就可以了
static inline size_t iheap_top(iheap_t *heap);
static inline void iheap_pop(iheap_t *heap);
static inline void iheap_push(iheap_t *heap, size_t index);
static inline void iheap_update(iheap_t *heap, size_t index);

static inline bool iheap_is_empty(iheap_t *heap) { return heap->size == 0; }

static inline bool iheap_is_full(iheap_t *heap) {
  return heap->size == heap->capacity;
}
static inline void iheap_fixdown(iheap_t *heap, size_t parent);
static inline void iheap_fixup(iheap_t *heap, size_t child);
static inline void iheap_check(iheap_t *heap);

// implementation
static inline void iheap_init(iheap_t *heap, void *value, size_t nvalue,
                              size_t type_size, size_t capacity,
                              bool (*predicate)(const void *lhs,
                                                const void *rhs)) {
  heap->value = value;
  heap->nvalue = nvalue;
  heap->type_size = type_size;
  heap->size = 0;
  heap->capacity = capacity;
  heap->predicate = predicate;

  // 然后我们要分配index数组和key数组
  heap->key = (size_t *)malloc(heap->capacity * sizeof(size_t));
  heap->index = (size_t *)malloc(heap->nvalue * sizeof(size_t));
  // 初始化为堆中没有任何节点
  for (size_t i = 0; i < heap->nvalue; ++i) {
    heap->index[i] = 0;
  }
}

static inline void iheap_clear(iheap_t *heap) { heap->size = 0; }
static inline void iheap_free(iheap_t *heap) {
  free(heap->key);
  free(heap->index);
}

// 为了更好的实现和理解 我们包装比较和交换两个函数
// 这两个函数的索引下标都是key数组的下标
// 也就是说 fixdown和fixup的逻辑是不知道底层是间接引用的
// 这样实现起来就简单了
// 然后这两个函数来隐式的处理间接引用
static inline void iheap_swap(iheap_t *heap, size_t left, size_t right) {
  // !!! 应该先更新index数组 不然bug了
  // 同时更新index数组
  // 这一块比较难解释 可以看我的笔记
  // 只要注意到 index通过key检索索引的元素始终是不变的 就很容易理解了
  heap->index[heap->key[left]] = right;
  heap->index[heap->key[right]] = left;

  // 咱们交换key数组的两个间接索引
  size_t temp = heap->key[left];
  heap->key[left] = heap->key[right];
  heap->key[right] = temp;
}

// 处理间接索引
static inline bool iheap_cmp(iheap_t *heap, size_t left, size_t right) {
  // 先通过间接索引拿到value数组中的索引
  size_t lhs = heap->key[left];
  size_t rhs = heap->key[right];
  // 然后根据 type_size计算出正确的内存位置
  void *lptr = (char *)(heap->value) + heap->type_size * lhs;
  void *rptr = (char *)(heap->value) + heap->type_size * rhs;
  // 然后通过用户提供的比较函数进行比较
  return heap->predicate(lptr, rptr);
}

static inline void iheap_fixdown(iheap_t *heap, size_t parent) {
  size_t left = 0;
  size_t right = 0;
  size_t child = 0;
  while ((left = iheap_left(parent)) <= heap->size) {
    // 我首先要在left和right中找出最小的哪一个
    // 如果只有左孩子，就只和左孩子比
    child = left;
    // 如果我们还有右孩子，就先比较两个孩子
    right = left + 1;
    if (right <= heap->size) {
      child = iheap_cmp(heap, left, right) ? left : right;
    }

    // 然后比较parent和child
    // 如果child比parent大 那么交换两者
    // 然后继续向下
    if (iheap_cmp(heap, parent, child)) {
      // 无事发生
      // 到这里函数就结束了呀
      return;
    } else {
      // 交换两者
      iheap_swap(heap, parent, child);
      // 用child替换parent继续向下修正堆
      parent = child;
    }
  }
}

static inline void iheap_fixup(iheap_t *heap, size_t child) {
  // 我们会不断的和parent去比 直到 parent, child 满足 cmp
  // 或者没有parent了
  size_t parent = 0;
  while ((parent = iheap_parent(child)) != 0 &&
         !iheap_cmp(heap, parent, child)) {
    // 交换parent和child
    iheap_swap(heap, parent, child);
    // 继续上修
    child = parent;
  }
}

static inline size_t iheap_top(iheap_t *heap) {
  return heap->key[iheap_root()];
}

static inline void iheap_pop(iheap_t *heap) {
  size_t top = iheap_top(heap);
  // 头尾交换
  iheap_swap(heap, iheap_root(), heap->size);
  // size - 1
  --heap->size;
  // 更新索引数组 因为这个元素已经被删了
  heap->index[top] = 0;
  // 然后启动一次下滤
  iheap_fixdown(heap, iheap_root());
}

static inline void iheap_push(iheap_t *heap, size_t index) {
  assert(!iheap_is_full(heap));
  // 从这两句话可以非常明显的看出来 key 和 index 是相反的
  // 先放到key的最后一个
  heap->key[heap->size + 1] = index;
  // 更新索引数组 指向最后一个位置
  heap->index[index] = heap->size + 1;
  // 增加heap size
  ++heap->size;
  iheap_fixup(heap, heap->size);
}

// 这个应该重写 和之前的逻辑不太一样的
static inline void iheap_update(iheap_t *heap, size_t index) {
  assert(index < heap->nvalue);
  size_t pos = heap->index[index];
  // 然后剩下的逻辑和之前一样
  size_t parent = iheap_parent(pos);
  // 不对啊 这里还要注意一些边界条件
  if (parent > 0 && !iheap_cmp(heap, parent, pos)) {
    return iheap_fixup(heap, pos);
  } else {
    return iheap_fixdown(heap, pos);
  }
}

static inline bool iheap_contain(iheap_t *heap, size_t index) {
  return heap->index[index] != 0;
}

// 最重要的iheap_check 最大的不同就是要检查三个数组的一致性
static inline void iheap_check(iheap_t *heap) {
  size_t child = 0;
  // 判断是否是一个二叉堆特别简单
  // 只需要遍历非叶子节点，然后判断一下他们的孩子是不是都比他们大或者小
  // 总之需要满足堆的性质
  // 而叶子节点是没有孩子的，所以是无需判断的，因为他们都是一个堆
  for (size_t i = iheap_root(); i <= heap->size / 2; ++i) {
    size_t left = iheap_left(i);
    if (left <= heap->size) {
      // 有左孩子
      assert(iheap_cmp(heap, i, left));
    }
    size_t right = iheap_right(i);
    if (right <= heap->size) {
      assert(iheap_cmp(heap, i, right));
    }
  }

  // 检查三个数组的一致性
  // 也就是检查那个不变式
  size_t size = 0;
  for (size_t i = 0; i < heap->nvalue; ++i) {
    if (heap->index[i] != 0) {
      // 也就是此索引对应的元素在堆中
      ++size;
      assert(heap->key[heap->index[i]] == i);
    }
  }
  assert(size == heap->size);
}

#endif // __INDEX_HEAP_H__
