// 2022/11/10
// zhangzhong
// binary search tree

#ifndef __TREE_H__
#define __TREE_H__

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef int64_t value_t;

typedef struct tree_node_t {
  value_t key;
  struct tree_node_t *left;
  struct tree_node_t *right;
  // 算法4 里面还包含了 size 表示以该节点为根的子树的节点数量
  // 不变量
  // size == left->size + right->size + 1
  size_t size;
} tree_node_t;

tree_node_t *make_tree_node(value_t key) {
  tree_node_t *node = (tree_node_t *)malloc(sizeof(tree_node_t));
  node->key = key;
  node->left = NULL;
  node->right = NULL;
  node->size = 1;
  return node;
}

// 但是为了用户的使用 我们需要定义一个 tree_t
typedef struct {
  tree_node_t *root;
  //   size_t size;
  //   size_t height;
} tree_t;

void tree_init(tree_t *tree) { tree->root = NULL; }

void tree_free(tree_t *tree) {
  // todo
}

tree_node_t *tree_search_impl(tree_node_t *root, value_t key) {
  while (root) {
    if (key < root->key) {
      root = root->left;
    } else if (key > root->key) {
      root = root->right;
    } else {
      return root;
    }
  }
  return NULL;
}

// search
bool tree_search(tree_t *tree, value_t key) {
  if (tree) {
    return tree_search_impl(tree->root, key) != NULL;
  }
  return false;
}

tree_node_t *tree_insert_impl(tree_node_t *root, value_t key) {
  // 逻辑和搜索是一样的
  while (root) {
    if (key < root->key) {
      if (root->left == NULL) {
        // 创建一个新节点 挂在left上
        root->left = make_tree_node(key);
        return root->left;
      } else {
        root = root->left;
      }
    } else if (key > root->key) {
      if (root->right == NULL) {
        // 创建一个新节点 挂在right上
        root->right = make_tree_node(key);
      } else {
        root = root->right;
      }
    } else {
      return root;
    }
  }

  // 如果到最后都找不到
}

void tree_insert(tree_t *tree, value_t key) {
  if (tree) {
    // 处理树为空时的特殊情况
    if (tree->root == NULL) {
      tree->root = make_tree_node(key);
    } else {
      tree_insert_impl(tree->root, key);
    }
  }
}

// max min
tree_node_t *tree_min_impl(tree_node_t *root) {
  tree_node_t *parent = root;
  while (root) {
    parent = root;
    root = root->left;
  }
  return parent;
}

tree_node_t *tree_max_impl(tree_node_t *root) {
  tree_node_t *parent = root;
  while (root) {
    parent = root;
    root = root->right;
  }
  return parent;
}

value_t tree_max(tree_t *tree) {
  assert(tree);
  assert(tree->root);
  tree_node_t *max_node = tree_max_impl(tree->root);
  assert(max_node);
  return max_node->key;
}

value_t tree_min(tree_t *tree) {
  assert(tree);
  assert(tree->root);
  tree_node_t *min_node = tree_min_impl(tree->root);
  assert(min_node);
  return min_node->key;
}

// todo
// 用max和min还可以实现两个迭代器操作
// 给定一个节点 找出它在树中的前驱节点 和 后继节点
// 前驱节点其实就是他的左子树的最大值的节点
// 后继节点就是右子树的最小节点
// 如果没有左子树 那么还需要考虑一些东西来着 算法导论里应该有

// 这个用迭代不好实现 就得用递归实现
tree_node_t *tree_floor_impl(tree_node_t *root, value_t key) {
  // case 1: 如果root->key == key 那么我们就直接找到了key
  // case 2
  // 那么我们要寻找的节点一定在左子树
  // case 3
  // key > root->key
  // 在这种情况下 我们要寻找的节点可能在右子树中
  // 如果右子树中存在小于等于key的节点 那么返回
  // 如果右子树中不存在 那么应该返回root
  if (!root) {
    return NULL;
  }

  if (key == root->key) {
    // case 1
    return root;
  } else if (key < root->key) {
    // case 2
    return tree_floor_impl(root->left, key);
  } else {
    // case 3
    tree_node_t *node = tree_floor_impl(root->right, key);
    if (node == NULL) {
      return root;
    } else {
      return node;
    }
  }
}

tree_node_t *tree_ceiling_impl(tree_node_t *root, value_t key) {
  if (!root) {
    return NULL;
  }

  // case 1
  if (key == root->key) {
    return root;
  } else if (key > root->key) {
    // case 2: 如果key > root->key 那么大于等于key的节点一定在右子树上
    return tree_ceiling_impl(root->right, key);
  } else {
    // case 3: 如果key < root->key 那么大于等于key的节点可能在左子树上
    // 或者就是根节点
    tree_node_t *node = tree_ceiling_impl(root->left, key);
    if (node == NULL) {
      return root;
    } else {
      return node;
    }
  }
}

// ceiling与floor正好相反 实现也是对称的

// 类似search操作 但是在找不到key的时候
// 返回小于key的最大值
// 这个函数是有可能没有结果的
value_t tree_floor(tree_t *tree, value_t key) {
  // floor的处理过程和search差不
  assert(tree);
}

// 四种搜索 前序 中序 后序 层序
typedef void (*tree_handle)(value_t key);

void tree_foreach_preorder_impl(tree_node_t *root, tree_handle handle) {
  if (root) {
    handle(root->key);
    tree_foreach_preorder_impl(root->left, handle);
    tree_foreach_preorder_impl(root->right, handle);
  }
}

void tree_for_each_preorder(tree_t *tree, tree_handle handle) {
  if (tree) {
    tree_foreach_preorder_impl(tree->root, handle);
  }
}

void tree_foreach_inorder_impl(tree_node_t *root, tree_handle handle) {
  if (root) {
    tree_foreach_inorder_impl(root->left, handle);
    handle(root->key);
    tree_foreach_inorder_impl(root->right, handle);
  }
}

void tree_foreach_inorder(tree_t *tree, tree_handle handle) {
  if (tree) {
    tree_foreach_inorder_impl(tree->root, handle);
  }
}

void tree_foreach_postorder_impl(tree_node_t *root, tree_handle handle) {
  if (root) {
    tree_foreach_postorder_impl(root->left, handle);
    tree_foreach_postorder_impl(root->right, handle);
    handle(root->key);
  }
}

void tree_foreach_postorder(tree_t *tree, tree_handle handle) {
  if (tree) {
    tree_foreach_postorder_impl(tree->root, handle);
  }
}

// next prev 这个没法实现 必须用parent指针才可以实现 在rbtree中有所体现

void tree_delete_min_impl(tree_node_t *root) {
  assert(root);
  tree_node_t *parent = root;
  while (root) {
    parent = root;
    root = root->left;
  }
  // 到这里就说明parent->left == NULL
  parent->left = NULL;
  free(root);
}

void tree_delete_max_impl(tree_node_t *root) {
  assert(root);
  tree_node_t *parent = root;
  while (root) {
    parent = root;
    root = root->right;
  }
  parent->right = NULL;
  free(root);
}

bool tree_is_leaf(tree_node_t *root) {
  return root->left == NULL && root->right == NULL;
}

void tree_delete_child(tree_node_t *parent, tree_node_t *child) {
  // 还可以检查是不是真的child
  assert(parent->left == child || parent->right == child);
  if (child->key < parent->key) {
    parent->left = NULL;
  } else {
    parent->right = NULL;
  }
}

void tree_next(tree_node_t *root, tree_node_t **parent, tree_node_t **next) {}

// delete
tree_node_t *tree_delete_impl(tree_node_t *root, value_t key) {
  // head
  tree_node_t *to_delete = root;
  tree_node_t *parent = to_delete;

  // 在删除之前，我们需要找到这个节点
  // 这里的逻辑应该和insert的逻辑是一样的
  // 只有处理的逻辑不同
  // 但是我们需要他的parent节点 所以才自己写的
  while (to_delete) {
    parent = to_delete;
    if (key < to_delete->key) {
      to_delete = to_delete->left;
    } else if (key > to_delete->key) {
      to_delete = to_delete->right;
    } else {
      break;
    }
  }

  // tree_node_t *to_delete = NULL;

  // 我们检查我们是否找到了这个节点
  if (to_delete != NULL) {
    // we found the node
    // so we need to delete it

    // case 1, to_delete is leaf
    if (tree_is_leaf(to_delete)) {
      // 直接删除，然后重置parent的指针
      to_delete = to_delete;
      // 这个操作看起来比较频繁的出现 可以包装一下
      // 就是删除孩子指针
      //   if (to_delete->element < parent->element) {
      //     parent->left = nullnode;
      //   } else {
      //     parent->right = nullnode;
      //   }
      tree_delete_child(parent, to_delete);
    }
    // case 2, to_delete only have one child
    else if (to_delete->left != NULL && to_delete->right == NULL) {
      // only have left child, just let the child be to_delete itself
      to_delete = to_delete;
      // 删除work，work只有左孩子 但是work是parent的左孩子
      // 所以将work的左孩子挂在parent的左孩子上
      if (to_delete->key < parent->key)
        parent->left = to_delete->left;
      else
        // 删除work，work只有左孩子 但是work是parent的右孩子
        // 将work的左孩子挂在parent的右孩子上
        parent->right = to_delete->left;
    } else if (to_delete->right != NULL && to_delete->left == NULL) {
      // 这两种情况是对称的
      // only have right child
      to_delete = to_delete;
      if (to_delete->key < parent->key)
        parent->left = to_delete->right;
      else
        parent->right = to_delete->right;
    } else if (to_delete->left != NULL && to_delete->right != NULL) {
      // case 3: 有两个孩子
      // case 3.1: 后继节点的父节点是将要删除的节点
      // case 3.2: 后继节点的父节点不是将要删除的节点
      // 不对，这里有一个问题，如果右子树只有一个值的时候！！！
      // 找到work的后继，其实就是work的右子树中的最小值
      // todo: 根据算法4 有两个孩子的时候应该随机的选择用前驱顶替或者后继顶替
      // 否则二叉树的平衡性将会变差

      // 我们需要一个函数，这个函数根据给定的节点，找到它的前驱prev和后继next
      // 因为work的右子树一定存在，所以这个调用是正确的
      // 这里需要返回两个值 一个是后继节点 一个是后继节点的父节点
      // 因为我们需要区分case 3.1 3.2
      tree_node_t *successor_parent = NULL;
      tree_node_t *successor = NULL;
      //   tree_node_t *node_pair = next(to_delete->right);
      //   successor_parent = node_pair.first;
      //   successor = node_pair.second;
      tree_next(to_delete, &successor_parent, &successor);

      // successor一定没有左孩子，但是它可能有右孩子
      // 判断successor一定是successor_parent的左孩子
      // 我们需要用successor的右孩子代替successor_parent的左孩子

      // 如果work的右子树只有一个节点，那么返回的successor_parent就是work
      // 在这种情况下
      // 好像是必须区分这种情况的，没办法避免， 在算法导论
      // p166-167中讨论了删除的四种情况 其中第三种和第四种分别对应这里的两个分支
      if (successor_parent == to_delete) {
        // case 3.1
        // 在这种情况下，直接用successor替换work
        // 更新两条边：一条是parent->to_delete --> parent->successor
        if (successor->key < parent->key)
          parent->left = successor;
        else
          parent->right = successor;
        // 更新完之后 successor的左指针也应该更新
        successor->left = successor_parent->left;
      } else {
        // case 3.2
        // 那我们需要用successor代替work的位置

        // step 1. 将successor从子树中删除 successor一定没有左孩子
        // 首先在work的右子树中删掉successor
        // 如果有parent指针的话，这个过程相当好实现
        // 如果没有的话，这个binary_next必须返回success的parent指针才行.
        // 所以删除操作和case2一样
        successor_parent->left = successor->right;

        // step 2. 更新相应的边
        // 梳理一下需要更新那些边
        // parent->to_delete --> parent->successor
        if (successor->key < parent->key)
          parent->left = successor;
        else
          parent->right = successor;
        // 因为successor顶替了work的位置
        // 所以他的left和right指针都应该更新成work的指针
        // 让successor代替work的位置
        // 我发现每次需要修改parent的child指针的时候，都需要重新判断是left还是right
        // 这个可以优化一下吗?
        successor->left = to_delete->left;
        successor->right = to_delete->right;
      }
    } else {
      // some error happends
    }
  }

  return to_delete;
}

// 搜索还有一个比较高级的 区间搜索 使用inorder traverse实现的 提供迭代器操作
// 暂不实现 算法4中还有三个操作 select rank 需要size支持 暂不实现

// 这个check是错误的

void tree_check(tree_t *tree) {
  assert(tree);

  // 1. 顺序: 左子树的所有节点都要小于等于根节点
  // 右子树的所有节点都要大于等于根节点
  // 2. size: size = left->size + right->size + 1 但是这个比较难维护呀
  // 而且没有意义
  // 这个检查可以使用递归实现 类似中序遍历

  // 还不如遍历一遍生成一个链表 然后检查顺序呢
}

#endif // __TREE_H__