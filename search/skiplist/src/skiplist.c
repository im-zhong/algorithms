// 2022/12/6
// zhangzhong
// skiplist

#include "skiplist/skiplist.h"
#include "util/c_random.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>

void skiplist_newlevel(skiplist_t *sl) {
  // 创建一层
  // 需要一个新的header
  skipnode_t *new_header = make_skipnode(sl, INT64_MIN);
  // new_header的down指向现在的header
  new_header->down = sl->header;
  // 右边指向tail
  // 所以现在的值的范围时 MIN -> MAX
  new_header->right = &sl->tail;
  // 然后更新sl
  sl->header = new_header;
  sl->level++;
}

void make_skiplist(skiplist_t *sl) {
  assert(sl);
  // 初始化随机数生成器
  sl->r = make_rand(0, 1);
  // 我们先包装bottom和tail
  sl->bottom.value = 0;
  sl->bottom.down = &sl->bottom;
  sl->bottom.right = &sl->tail;
  // 包装tail
  sl->tail.value = INT64_MAX;
  sl->tail.down = &sl->bottom;
  sl->tail.right = &sl->tail;
  // 不创建第一层
  sl->level = 0;
  sl->header = &sl->bottom;
  //   skiplist_newlevel(sl);
}

skipnode_t *make_skipnode(skiplist_t *sl, value_t value) {
  skipnode_t *node = (skipnode_t *)malloc(sizeof(skipnode_t));
  node->value = value;
  node->right = &sl->tail;
  node->down = &sl->bottom;
  return node;
}

void free_skiplist(skiplist_t *skiplist) {}

// 用tail和bottom简化编程
// 每一行的最后都指向一个tail 而tail的值是正无穷
// 就不用判断哪些边界条件了
bool skiplist_search(skiplist_t *sl, value_t value) {
  sl->bottom.value = value;
  skipnode_t *work = sl->header;

  // 在每一层中寻找一对链表节点
  // n1 -> n2
  // n1 -> value -> n2
  // 然后从两者之间下去
  // 一层一层的写
  // 好写和好理解
  while (work->value != sl->bottom.value) {
    // 找到value在本层适当的位置
    // 由结构决定的条件：在每一层开始的时候，work->value 一定 小于 value
    assert(work->value < value);
    // 然后这个结构需要循环 一直到中间的条件触发
    // while (true) {
    //   // 所以我只需要和右边的值进行比较
    //   // 有三种可能
    //   //   if (value == work->right->value) {
    //   //     return true;
    //   //   } else if (value < work->right->value) {
    //   //     // 那么现在的work就是下降点
    //   //     // work->value < value < work->right->value
    //   //     break;
    //   //   } else {
    //   //     // value > work->right->value
    //   //     // 此时需要向右
    //   //     work = work->right;
    //   //   }
    // }

    for (; value >= work->right->value; work = work->right)
      ;
    // 跳出循环的时候就是 work->value <= value < work->right->value
    // 然后我们就需要判断
    if (work->value == value) {
      return true;
    }
    // work->value < value < work->right->value
    // 现在的work就是下降点
    work = work->down;
  }

  return work != &sl->bottom;
}

// 实现一个random level
// 插入第一层的概率是 1/2 插入一二层的概率是 1/4 插入一二三层的概率是1/8
// 以此类推 概率指数减少

// 他会返回 1 - max_level
int random_level(skiplist_t *sl) {
  // 不断的生成随机数
  int level = 1;
  while (crand(&sl->r) == 0)
    ++level;
  return level < MAX_LEVEL ? level : MAX_LEVEL;
}

// 算了 还是不想的复杂了 就实现一个简单版本的skiplist吧
// 提供三个简单的操作
void skiplist_insert(skiplist_t *sl, value_t value) {
  // 首先我们会生成一个random level
  int level = random_level(sl);

  // 哎 但是我怎么知道现在多少层呢??
  // 那就得保存一个level变量 了 有点麻烦了
  // 首先检查生成的level是否变大了
  while (level > sl->level) {
    // 有点麻烦呀
    // 有一些边界情况需要处理呀
    // 我们就创建新的一层
    // new_level()
    // 然后把 level ++
    skiplist_newlevel(sl);
  }

  // 从顶层开始插入此元素 如果到了需要建立索引的层 需要在合适的位置插入元素
  // 同时保留下一层的元素指针 因为我们需要建立 down 索引
  // 我们有一个指针保存顶层链表头
  sl->bottom.value = value;
  skipnode_t *work = sl->header;
  int current_level = sl->level;
  // 没有人会关心tail的down是谁
  // 就像没有人会关心bottom的值一样
  // 我们让up = tail
  // 可以省掉下面的if判断
  skipnode_t *up = &sl->tail;
  // 一层一层的写 一个大循环就是处理一层
  // 一层一层的写从逻辑上更清晰一些
  // 外面一个大循环是一层
  // 大循环内的循环处理一层内的逻辑
  while (work->value != sl->bottom.value) {

    // 我们首先找到value在该层合适的位置
    // 在每个循环开始 work总是指向该层链表的开始搜索的位置
    assert(work->value < value);
    for (; value >= work->right->value; work = work->right)
      ;
    assert(work->value <= value);
    assert(value < work->right->value);
    // 跳出循环时 满足下面的关系式
    // work->value <= value < work->right->value

    // 如果我们找到了对应的值
    if (value == work->value) {
      // 这意味着我们不需要再向下插入了
      //   if (up) {
      //     // 如果此时up有效 可能是一种特殊的情况
      //     // 就是待插入的值存在 但是新产生的level值比它大
      //     // 所以我们再上层建立了新的索引
      //     up->down = work;
      //   }
      up->down = work;
      return;
    }

    // 现在肯定满足这个条件
    // work->value < value < work->right->value

    // 如果需要在该层建立索引
    if (current_level <= level) {
      // 在这一层我们就需要插入元素了
      // 我们需要将元素插入这个有序链表 并返回插入元素的前一个链表节点
      // 这样我们好从这里向下

      // 在插入的时候 我们就需要考虑是否需要建立节点了
      // 我们需要在这里插入节点
      skipnode_t *new_node = make_skipnode(sl, value);
      new_node->right = work->right;
      work->right = new_node;
      // new_node->down 需要到下一层才能更新
      // 这里以防万一先写成bottom
      // new_node->down = &sl->bottom;
      // 如果上一层有索引的话 那么应该在这里更新他的down指针
      //   if (up) {
      //     up->down = new_node;
      //   }
      up->down = new_node;
      up = new_node;
    }

    // 前往下一层
    work = work->down;
    current_level--;
  }
}

void skiplist_delete(skiplist_t *sl, value_t value) {
  // 删除的过程需要首先找到该节点
  // 将所有链表中找到的节点值都删掉即可
  // 删完之后最顶上的链表可能变成空的 就是只有 header -> tail
  // 此时可以降级
  sl->bottom.value = value;
  skipnode_t *work = sl->header;
  skipnode_t *tmp = NULL;

  // 在每一层中寻找一对链表节点
  // n1 -> n2
  // n1 -> value -> n2
  // 然后从两者之间下去
  // 一层一层的写
  // 好写和好理解
  while (work->value != sl->bottom.value) {
    // 找到value在本层适当的位置
    // 由结构决定的条件：在每一层开始的时候，work->value 一定 小于 value
    assert(work->value < value);
    for (; value > work->right->value; work = work->right)
      ;
    // 跳出循环的时候就是 work->value < value <= work->right->value
    assert(work->value < value);
    assert(value <= work->right->value);

    if (work->right->value == value) {
      // 这说明我们找到了这个节点，我们需要删除此节点
      // 卧槽 但是删除需要在前一个节点删啊 否则 没法找到前驱了
      tmp = work->right;
      work->right = work->right->right;
      // 不对不对，这个work->right已经被改了,cao
      // bug
      // free(work->right);
      free(tmp);
    }
    // work->value < value < work->right->value
    // 现在的work就是下降点
    work = work->down;
  }

  // 最顶层是空的 可以删掉该层
  skipnode_t *header = sl->header;
  while (header != &sl->bottom && header->right == &sl->tail) {
    sl->header = header->down;
    --sl->level;
    free(header);
    header = sl->header;
  }
}

// check skiplist

void check_skiplist(skiplist_t *sl) {
  // 基本的思路还是遍历整张sl
  // 其中每一层都需要有序
  // 只要不是最后一层，down指针必然指向value相同的下层节点
  assert(sl);
  skipnode_t *header = sl->header;
  int level = sl->level;
  while (header != &sl->bottom) {
    // 遍历整个链表
    // 每个节点的名字就是 "level/value"
    for (skipnode_t *work = header; work != &sl->tail; work = work->right) {
      // [level/work] -> [level/work->right]
      assert(work->value < work->right->value);
      // 可以用level来判断呀
      if (level != 1) {
        assert(work->down != &sl->bottom);
        assert(work->down != work);
        assert(work->down->value == work->value);
      } else {
        assert(work->down == &sl->bottom);
      }
    }
    // 遍历完这一层，前往下一层
    header = header->down;
    level--;
  }
}
