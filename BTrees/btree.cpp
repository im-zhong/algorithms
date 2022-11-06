// 2022/11/5
// zhangzhong
// B Tree

#include "btree.h"
#include "array.h"
#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <type_traits>

static bool btree_is_full_node(btree_node_t *node) {
  return node->size == MINIMUM_DEGREE * 2 - 1;
}

static btree_node_t *btree_allocate_node() {
  btree_node_t *node = (btree_node_t *)malloc(sizeof(btree_node_t));
  assert(node);
  // 初始化node的内容啊
  node->is_leaf = false;
  node->size = 0;
  for (size_t i = 0; i < MINIMUM_DEGREE * 2 - 1; ++i) {
    node->keys[i] = 0;
  }
  for (size_t i = 0; i < MINIMUM_DEGREE * 2; ++i) {
    node->childs[i] = nullptr;
  }
  return node;
}

void btree_init(btree_t *btree) { btree->root = nullptr; }

bool btree_is_empty(btree_t *btree) { return btree->root == nullptr; }

static void btree_free_impl(btree_node_t *root) {
  // 最简单的方法就是递归呀
  // 我们就遍历所有的child 然后调用就可以了
  if (!root) {
    return;
  }
  if (root->is_leaf) {
    // 那么我们就可以被删掉了呀
    return;
  } else {
    // 那么我们需要把所有的节点都删除掉之后 才能返回
    for (size_t i = 0; i < root->size + 1; i++) {
      btree_free_impl(root->childs[i]);
      free(root->childs[i]);
    }
  }
}

void btree_free(btree_t *btree) {
  // free(root);
  if (btree) {
    btree_free_impl(btree->root);
    free(btree->root);
  }
}

static btree_node_t *btree_search_impl(btree_node_t *root, value_t key) {
  if (!root) {
    return NULL;
  }

  // 遍历当前节点的关键字
  size_t pos = 0;
  if (array_search(root->keys, root->size, key, &pos)) {
    // 找到了
    // 我们可以返回包含关键字对应的节点指针
    return root;
  }

  // 如果没有找到 并且我们是leaf 那么就返回空指针
  if (root->is_leaf) {
    return NULL;
  }

  // 否则 我们就递归查找
  return btree_search_impl(root->childs[pos], key);
}

bool btree_search(btree_t *btree, value_t key) {
  if (btree) {
    btree_node_t *node = btree_search_impl(btree->root, key);
    return node != NULL;
  }
  return false;
}

// 先实现一个split操作
// 要对root节点的i子节点进行split操作
// i子节点必须是满节点
// 我们会把i子节点分成两个关键字有 t -1 的节点
// 然后root节点增加keys和childs
static void btree_split(btree_node_t *root, value_t pos) {
  // 要将child i 进行一次分裂
  // child i 是一个满节点
  assert(!btree_is_full_node(root));
  assert(btree_is_full_node(root->childs[pos]));
  // 1. 找到中间下标 是 MINIMUM_DEGREE - 1

  btree_node_t *child = root->childs[pos];

  btree_node_t *new_node = btree_allocate_node();
  new_node->is_leaf = child->is_leaf;

  // 2. 然后更新两个节点的key和childs
  new_node->size = MINIMUM_DEGREE - 1;
  for (value_t i = 0; i < new_node->size; ++i) {
    // copy keys from right part of original child node
    // except the middle key
    new_node->keys[i] = child->keys[i + MINIMUM_DEGREE];
  }
  // copy the childs
  if (!child->is_leaf) {
    for (value_t i = 0; i < new_node->size + 1; ++i) {
      new_node->childs[i] = child->childs[i + MINIMUM_DEGREE];
    }
  }

  // 更新child的数据
  value_t split_key = child->keys[MINIMUM_DEGREE - 1];
  child->size = MINIMUM_DEGREE - 1;

  // 然后是更新root的keys 因为我们需要加入一个新的key
  // 然后是更新root的childs 因为新加入了一个孩子节点
  for (size_t i = root->size; i > pos; --i) {
    root->keys[i] = root->keys[i - 1];
  }
  root->keys[pos] = split_key;

  for (size_t i = root->size + 1; i > pos + 1; --i) {
    root->childs[i] = root->childs[i - 1];
  }
  root->childs[pos + 1] = new_node;

  // 因为更新了三个节点 所以对应了三次磁盘写，可以合并成一次
  // 最后更新 root->size 因为我们新加入了一个关键字
  root->size++;
}

// merge 我有一个root 我还要指定一个 pos
// 把posright_child 一起合并到left_child里面
// 并且正确的更新所有的数据 并且释放right_child
static void btree_merge(btree_node_t *root, value_t pos) {
  // 就是要把pos对应的两个左右孩子加上pos对应的关键字放到一块
  // 我们的两个子节点的关键字数量一定都得是 t-1
  btree_node_t *left_child = root->childs[pos];
  btree_node_t *right_child = root->childs[pos + 1];
  assert(left_child->size == MINIMUM_DEGREE - 1);
  assert(right_child->size == MINIMUM_DEGREE - 1);

  // 把右孩子里面的key和孩子都复制到左孩子
  // for (size_t i = 0; i < MINIMUM_DEGREE - 1; ++i) {
  //   left_child->childs[i + MINIMUM_DEGREE] = right_child->childs[i];
  // }
  array_insert_array((value_t *)left_child->childs, left_child->size + 1,
                     (value_t *)right_child->childs, right_child->size + 1,
                     left_child->size + 1);
  // 把pos对应的key放到left_child最右边
  left_child->keys[left_child->size] = root->keys[pos];
  left_child->size++;
  // 把right_child的所有关键字复制到left_child最右边
  // for (size_t i = 0; i < right_child->size; ++i) {
  //   left_child->keys[i + MINIMUM_DEGREE] = right_child->keys[i];
  // }
  array_insert_array(left_child->keys, left_child->size, right_child->keys,
                     right_child->size, left_child->size);
  // 更新size
  left_child->size = MINIMUM_DEGREE * 2 - 1;

  // 从root中删除关键字和right_child孩子指针
  array_delete(root->keys, root->size, pos);
  array_delete((value_t *)root->childs, root->size + 1, pos + 1);
  // 更新size
  root->size--;
  // 释放掉right_child
  free(right_child);
}

void btree_insert_impl(btree_node_t *root, value_t key) {
  size_t pos = 0;
  bool r = array_search(root->keys, root->size, key, &pos);
  if (r) {
    // 如果找到了关键字 直接返回就好了
    return;
  }

  if (root->is_leaf) {
    // 如果我们是叶子节点，那么我们直接将key插入keys中
    // 为什么不会满呢??
    array_insert(root->keys, root->size, key);
    root->size++;

  } else {
    // 如果我们不是叶子节点
    // 那么顺序遍历我们的关键字 查看应该向那个叶子节点递归插入
    // 同时应该注意对应的叶子节点是否已满

    btree_node_t *child = root->childs[pos];
    // 假设我们找到了正确的子节点 i
    // 如果子节点是满的 那么先分裂
    if (btree_is_full_node(child)) {
      btree_split(root, pos);

      // 分裂完了之后，我们应该插在哪儿呢
      // 我们应该与新提升上来的关键字比较
      // 然后适当的修改i
      // 这里不应该对child进行递归 还是应该对root进行递归
      // 我们在split结束之后 你无法知道应该向那个child进行插入
      btree_insert_impl(root, key);
    } else {
      btree_insert_impl(child, key);
    }
  }
}

void btree_insert(btree_t *btree, value_t key) {
  if (!btree) {
    return;
  }

  btree_node_t *root = btree->root;

  // first check if root is nullptr
  if (root == nullptr) {
    btree_node_t *new_node = btree_allocate_node();
    new_node->is_leaf = true;
    new_node->size = 1;
    array_insert(new_node->keys, 0, key);
    btree->root = new_node;
    return;
  }

  // check if root is full
  // 在这种情况下 我们的高度会增加
  if (btree_is_full_node(btree->root)) {
    // 如果根节点满了，那么我们想要插入一个节点之前就必须分裂根节点
    // 这是B树增加高度的唯一途径
    // 这个是新的root
    btree_node_t *new_root = btree_allocate_node();
    new_root->is_leaf = false;
    // 然后当当前的root作为新root的child
    // 因为目前我们只有一个孩子 所以关键字数量是零
    new_root->size = 0;
    new_root->childs[0] = btree->root;
    btree->root = new_root;
    // 接下来就是分裂
    btree_split(new_root, 0);

    // split之后，应该从split的当前节点开始 重新开始递归插入过程
    root = new_root;
  }

  btree_insert_impl(root, key);
}

value_t btree_max_impl(btree_node_t *root) {
  assert(root);
  while (root) {
    // 如果是叶子节点，直接返回最右边的值
    if (root->is_leaf) {
      assert(root->size > 0);
      return root->keys[root->size - 1];
    }
    // 如果不是 我们应该从右往左遍历孩子节点
    // 从不是nullptr的第一个孩子节点向下 直到叶子节点
    for (size_t i = root->size; i >= 0; --i) {
      if (root->childs[i]) {
        root = root->childs[i];
        break;
      }
    }
  }
  assert(false);
}

// min和max差不多，唯一的区别就是最小值放在最左边
value_t btree_min_impl(btree_node_t *root) {
  assert(root);
  while (root) {
    // 如果是叶子节点，直接返回最左边的值
    if (root->is_leaf) {
      assert(root->size > 0);
      return root->keys[0];
    }
    // 如果不是 我们应该从左往右遍历孩子节点
    // 从不是nullptr的第一个孩子节点向下 直到叶子节点
    for (size_t i = 0; i < root->size + 1; ++i) {
      if (root->childs[i]) {
        root = root->childs[i];
        break;
      }
    }
  }
  assert(false);
}

void btree_delete_impl(btree_node_t *root, value_t key) {
  // 首先在本节点的key中查找key
  // case1: 找到了key 并且我们是叶子节点
  size_t pos = 0;
  bool r = array_search(root->keys, root->size, key, &pos);
  if (r && root->is_leaf) {
    // 那么我们直接将之从叶子节点里面删除即可
    array_delete(root->keys, root->size, pos);
    return;
  }

  // case2 关键字在当前节点中，但是我们不是叶子节点
  if (r && !root->is_leaf) {
    // 给定任意一个关键字的下标 我们一定有前面和后面两个孩子
    // 首先查看前面的孩子
    btree_node_t *left_child = root->childs[pos];
    btree_node_t *right_child = root->childs[pos];
    // 如果我们的左孩子至少包含t个关键字
    if (left_child && left_child->size >= MINIMUM_DEGREE) {
      // 在左孩子中找到当前key的前驱key'
      // 递归的删除key' 并且在root中用key'代替key即可

      // 关键是如何找到前驱
      // 根据树形结构，前驱其实就是最右边的叶子的最右边的值
      // 或者说，是当前子树的最大值
      // 我应该实现两个函数，分别是最大值和最小值
      value_t predecessor_key = btree_max_impl(left_child);
      // value_t successor_key = btree_min_impl(root);

      // 要做的是用predecessor替换当前的key
      // 然后递归的删除predecessor_key
      btree_delete_impl(root, predecessor_key);
      root->keys[pos] = predecessor_key;

    } else if (right_child && right_child->size >= MINIMUM_DEGREE) {
      value_t successor_key = btree_min_impl(right_child);
      // ??? 应该从哪个节点开始删除呢???
      btree_delete_impl(right_child, successor_key);
      root->keys[pos] = successor_key;

    } else {
      // 合并两个节点
      // 在3中也用到了合并这个功能
      // 我应该写一个函数
      // 我其实有一个疑问那 为什么这里一定至少有t-1个节点呢
      assert(left_child);
      assert(right_child);
      assert(left_child->size == MINIMUM_DEGREE - 1);
      assert(right_child->size == MINIMUM_DEGREE - 1);

      // merge 我有一个root 我还要指定一个 pos
      // 把posright_child 一起合并到left_child里面
      // 并且正确的更新所有的数据 并且释放right_child
      btree_merge(root, pos);
      // 然后递归的删除key
      btree_delete_impl(left_child, key);
    }

    return;
  }

  // case3 关键字不在当前节点中
  // 那么我们需要向下寻找吗
  // 但是在向下寻找的过程中需要排除两种特殊的情况 然后再向下
  btree_node_t *child = root->childs[pos];
  if (!child) {
    // 如果child指针无效 说明要删除的数据是不存在的
    // 直接返回就好了
    return;
  }
  // 就是说我们有最少的允许的关键字数量
  // 那么我们需要调整之后 才能继续向下递归删除
  // 必须执行额外的步骤 保证至少包含t的关键字 才能继续下降
  if (child->size == MINIMUM_DEGREE - 1) {
    // case 3.1 如果我们有一个相邻的兄弟，包含至少t个关键字
    // 检查是否有左兄弟, 检查左兄第是否存在，检查左兄弟的关键字数量至少为t
    if (pos > 0 && root->childs[pos - 1] &&
        root->childs[pos - 1]->size >= MINIMUM_DEGREE) {
      // 将root中的一个关键字降至child中，将兄弟节点的一个关键字升至root中
      // 将兄弟中的相应的孩子指针移动到child中，
      // 这样child就获得了一个额外的关键字

      // 兄弟的孩子指针
      btree_node_t *left_sibling = root->childs[pos - 1];
      // 需要兄弟把最右边的关键字提升到root 替换pos对应的关键字
      // 然后把最右边的孩子给child放到最左边

      // 将root中的pos对应的关键字放到child中
      array_insert(child->keys, child->size, root->keys[pos]);
      // 将兄弟的孩子拿过来
      for (size_t i = child->size + 1; i > 0; --i) {
        child->childs[i] = child->childs[i - 1];
      }
      child->childs[0] = left_sibling->childs[left_sibling->size];
      // 一定要在更新完keys和childs之后再更新size
      child->size++;

      // 替换root的关键字
      root->keys[pos] = left_sibling->keys[left_sibling->size - 1];

      // 删除兄弟节点的一个关键字
      left_sibling->size--;

    } else if (pos < root->size && root->childs[pos + 1] &&
               root->childs[pos + 1]->size >= MINIMUM_DEGREE) {
      // 检查是否有右兄弟，关键字数量至少为t
      btree_node_t *right_sibling = root->childs[pos + 1];
      // 和上面非常类似 其实就是对称的

      // 把root中的关键字放到child的最右边
      child->keys[child->size] = root->keys[pos];
      // 把兄弟的最左边孩子拿过来 放到child的最右边
      child->childs[child->size + 1] = right_sibling->childs[0];
      // 更新child的size
      child->size++;

      // 替换root的关键字
      root->keys[pos] = right_sibling->keys[0];

      // 删除右兄弟的第一个关键字和第一个孩子
      array_delete(right_sibling->keys, right_sibling->size, 0);
      // 删除第一个孩子
      for (size_t i = 0; i < right_sibling->size; ++i) {
        right_sibling->childs[i] = right_sibling->childs[i + 1];
      }
      // 更新size
      right_sibling->size--;

    } else {
      // case 3.2 左右兄弟都只有t-1个关键字
      // 将兄弟合并 关键字放下去
      // 然后再递归的遍历就好了
      btree_merge(root, pos);
    }
  }
  // 最后就是递归的删除key
  btree_delete_impl(child, key);
}

void btree_delete(btree_t *btree, value_t key) {
  if (!btree->root) {
    return;
  }
  btree_delete_impl(btree->root, key);
}

// 我想实现一个遍历的函数
// 它可以把key按照从小到大的顺序输出出来
