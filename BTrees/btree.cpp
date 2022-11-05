// 2022/11/5
// zhangzhong
// B Tree

#include "btree.h"
#include "array.h"
#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

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

static btree_node_t *btree_search_impl(btree_node_t *root, int key) {
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

bool btree_search(btree_t *btree, int key) {
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
static void btree_split(btree_node_t *root, int pos) {
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
  for (int i = 0; i < new_node->size; ++i) {
    // copy keys from right part of original child node
    // except the middle key
    new_node->keys[i] = child->keys[i + MINIMUM_DEGREE];
  }
  // copy the childs
  if (!child->is_leaf) {
    for (int i = 0; i < new_node->size + 1; ++i) {
      new_node->childs[i] = child->childs[i + MINIMUM_DEGREE];
    }
  }

  // 更新child的数据
  int split_key = child->keys[MINIMUM_DEGREE - 1];
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

static void btree_merge() {}

void btree_insert_impl(btree_node_t *root, int key) {
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

void btree_insert(btree_t *btree, int key) {
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

void btree_delete_impl(btree_node_t *root, int key) {
  // 首先在本节点的key中查找key
  // case1: 找到了key 并且我们是叶子节点
  size_t i = 0;
  array_search(root->keys, root->size, key, &i);
  if (i < root->size && root->is_leaf) {
    // 那么我们直接将之从叶子节点里面删除即可
    array_delete(root->keys, root->size, i);
    return;
  }

  // case2 关键字在当前节点中，但是我们不是叶子节点
  if (i < root->size && !root->is_leaf) {
  }
}

void btree_delete(btree_t *btree, int key) {
  if (!btree->root) {
    return;
  }
  btree_delete_impl(btree->root, key);
}

// test
// bool btree_check(btree_t *btree) {
//   // 检查这棵树是不是一颗b树
//   // 怎么检查呢
//   // 1. 很明显就是节点孩子数量的检查
//   // 2. 还有一些数据结构一致性上的检查
//   // 2.1 leaf 状态 只有叶子节点是leaf 其他的不是leaf
//   // 2.2 还可以检查size 所有节点的size和总的size应该是一样的
//   // 3.3 还有顺序
//   if (!btree) {
//     return false;
//   }

//   if (btree->root) {
//   }
// }

// 这个功能最好是用C++写呀
// void dot(btree_t *btree) {
//   // 遍历这棵树 生成dot图
//   char text[4096] = {'\0'};
//   size_t pos = 0;
//   strcpy(text, const char *)
// }
