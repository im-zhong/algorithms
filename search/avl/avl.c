// 2022/11/20
// zhangzhong
// avl implementation

#include "avl.h"
#include <assert.h>
#include <container/test.h>
#include <container/vector.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

avl_t make_avl() {
  avl_t avl;
  avl.root = NULL;
  return avl;
}

// 暂时不考虑内存相关的代码
void free_avl(avl_t *avl) {}

// avl_search

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

bool avl_search(avl_t *avl, value_t value) {
  if (avl) {
    return tree_search_impl(avl->root, value) != NULL;
  }
  return false;
}

// bool avl_is_empty

bool avl_is_empty(avl_t *avl) { return avl->root == NULL; }
void avl_clean(avl_t *avl) {}

// avl_insert

tree_node_t *make_tree_node(value_t value) {
  tree_node_t *node = (tree_node_t *)malloc(sizeof(tree_node_t));
  node->key = value;
  node->left = NULL;
  node->right = NULL;
  // 任何一个新创建的节点高度应该是1才对
  node->height = 1;
  return node;
}

static size_t tree_height(tree_node_t *root) {
  if (root) {
    return root->height;
  } else {
    return 0;
  }
}

static void tree_update_height(tree_node_t *root) {
  root->height = max(tree_height(root->left), tree_height(root->right)) + 1;
}

// rotate

// 这样我可以使用 static来防止接口暴漏

// 实现avl树需要实现四种旋转操作
// 这些操作是avl和红黑树公用的 所以可以之后可以重构一下
// 可以借用红黑树中实现的trick 用nullnode
// left rotate at node
//
//        (root)
//  (left)   (right)
//  (A)  (B)
//
//   (left)
//  (A) (root)
//      (B)  (right)
// 从编程实现上讲，我们还需要更新node->parent的指针
// 貌似从外部更新是更好的选择 我们可以返回 new root
static tree_node_t *rotate_right(tree_node_t *root) {
  assert(root);
  tree_node_t *left = root->left;
  tree_node_t *right = root->right;

  // 修正child指针
  root->left = left->right;
  left->right = root;

  // 修正高度
  tree_update_height(root);
  tree_update_height(left);

  // left作为new root
  // left的parent指针需要在外面改
  return left;
}

// 要正确的更新高度
// rotate left
//
//    root
// left  right
//       A   B
//
//     right
//   root  B
// left A
static tree_node_t *rotate_left(tree_node_t *root) {
  assert(root);
  tree_node_t *left = root->left;
  tree_node_t *right = root->right;

  // 修正child指针
  root->right = right->left;
  right->left = root;

  // 修正height
  tree_update_height(root);
  tree_update_height(right);

  // right作为新的根
  return right;
}

// 双旋转
// 先不实现 看看insert和delete需要如何调用
// todo: 别忘了更新height
static tree_node_t *rotate_left_right(tree_node_t *root) {
  // 双旋转其实就是两次单旋转
  //  root
  // A
  //   B
  root->left = rotate_left(root->left);
  tree_update_height(root);
  return rotate_right(root);
}

static tree_node_t *rotate_right_left(tree_node_t *root) {
  // root
  //      A
  //  B
  root->right = rotate_right(root->right);
  tree_update_height(root);
  return rotate_left(root);
}

// balance

tree_node_t *avl_balance_left(tree_node_t *root) {
  assert(root);
  assert(tree_height(root->left) == tree_height(root->right) + 2);
  // 以root为根的左子树 不平衡
  // 也就是左子树的高度 = 右子树的高度 + 2
  // 所以需要平衡

  // 左子树的高度至少是2，因为右子树的高度最小是0
  // 左子树的高度至少是2，那么root一定有左孩子 X
  // 左孩子也至少有一个孩子 Y
  // 我们只需要观察X Y的结构就可以直到如何旋转了
  // case 1
  //   root
  //  X
  // Y
  // 我们就执行一次单旋转
  // case 2
  //  root
  // X
  //   Y
  // 我们就执行一次双旋转

  if (root->left->left != NULL) {
    // 单旋转
    return rotate_right(root);
  } else {
    // 双旋转
    return rotate_left_right(root);
  }
}

// 和上面对称
tree_node_t *avl_balance_right(tree_node_t *root) {
  assert(root);
  assert(tree_height(root->right) == tree_height(root->left) + 2);

  if (root->right->right != NULL) {
    // 单旋转
    // 如果这样写是正确的话 那就还有最后一个问题了
    // 就是旋转的命名
    // 我们应该如何命名这些旋转？？是按照待旋转的节点位置？？还是旋转的方向？？
    // 看看三本算法书是怎么说的
    // 得了，所有的书都是按照方向来说的 就是
    // 右右 这种结构对应向左旋转一次 这样的操作就叫做 rotate_left
    return rotate_left(root);
  } else {
    // 双旋转
    return rotate_right_left(root);
  }
}

static void check_balance_impl(tree_node_t *root) {
  assert(root);
  assert(tree_height(root->left) == tree_height(root->right) ||
         tree_height(root->left) == tree_height(root->right) + 1 ||
         tree_height(root->right) == tree_height(root->left) + 1);
}

static void check_balance(tree_node_t *root) {
  if (root) {
    check_balance_impl(root);
    check_balance(root->left);
    check_balance(root->right);
  }
}

// 实现一个balance操作
// 检查以root为根的子树是否不平衡，如果不平衡，做平衡操作
// 返回新树的根
tree_node_t *avl_balance(tree_node_t *root) {
  assert(root);
  // step 1. 检查平衡条件
  if (tree_height(root->left) == tree_height(root->right) + 2) {
    // 左子树更高
    // 在平衡结束之后 应该进行检查
    // 平衡之后的子树的根作为新的根
    root = avl_balance_left(root);
  } else if (tree_height(root->right) == tree_height(root->left) + 2) {
    // 右子树更高
    root = avl_balance_right(root);
  }
  check_balance_impl(root);
  return root;
}

// avl insert

// 递归实现的 比较简单
// 插入分四种情况，对称的，其实就对应四种旋转操作
// 在插入完毕之后，自底向上搜索平衡条件不能维持的节点
// 按照类型执行一次相应的旋转操作即可
// 这个函数的语义应该是在以root为根的子树中插入一个节点 并返回子树的根
tree_node_t *avl_insert_impl(tree_node_t *root, value_t key) {
  // 我们要求root必须非空 按照这样的逻辑进行
  // 否则添加节点时 我们需要判断parent 的 child指针方向 这多了一次检测
  assert(root);

  if (key == root->key) {
    // 那这就无事发生
    // 递归结束条件1
  } else if (key < root->key) {
    // 那么我们需要递归的向左子树插入
    // 但是需要首先判断左子树是否为空
    if (root->left) {
      root->left = avl_insert_impl(root->left, key);
    } else {
      // 那么我们需要在这里插入一个新的节点
      // 递归结束条件2
      tree_node_t *node = make_tree_node(key);
      root->left = node;
    }
  } else {
    // key > root->key
    if (root->right) {
      root->right = avl_insert_impl(root->right, key);
    } else {
      // 递归结束条件3
      tree_node_t *node = make_tree_node(key);
      root->right = node;
    }
    // 如果我们有子树 那么高度就是 max height of subtree + 1
    // 如果没有子树，就是0
  }

  tree_update_height(root);
  // 平衡之后可能会产生新的根
  root = avl_balance(root);
  return root;
}

// 然后是允许链接的
// 我们应该把实现放在开头的位置
// 内部实现放在下面
// insert实现起来和普通的二叉树在最开始没有区别
// 但是插入之后 我们需要更新树的高度 之后还需要重新平衡
void avl_insert(avl_t *avl, value_t value) {
  assert(avl);

  // case 1: if empty
  if (avl_is_empty(avl)) {
    // create a new node
    avl->root = make_tree_node(value);
    // tree_update_height(avl->root);
  } else {
    // case 2: not empty
    // 递归函数需要采用这种写法 让函数返回子树的根 这样实现起来就简单多了
    avl->root = avl_insert_impl(avl->root, value);
    // 这个update height貌似是不需要的
    // tree_update_height(avl->root);
  }
}

// avl delete

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

// 返回的是 在以root为根的子树中 删除最小节点后的新产生的子树的根
// 都得写成递归的 因为我们需要自底向上修改每一个节点的height
// 这里的问题就是在delete_min和delete max里面都需要考虑balance操作
tree_node_t *tree_delete_min_impl(tree_node_t *root) {
  if (root->left == NULL) {
    // 因为root就是min 所以之后会被删掉 所以不需要更新height
    // 递归结束条件
    return root->right;
  } else {
    root->left = tree_delete_min_impl(root->left);
    // 然后更新高度
    tree_update_height(root);
    return avl_balance(root);
  }
}

tree_node_t *tree_delete_max_impl(tree_node_t *root) {
  if (root->right == NULL) {
    return root->left;
  } else {
    root->right = tree_delete_max_impl(root->right);
    tree_update_height(root);
    return avl_balance(root);
  }
}

// 算法4是递归删除的
tree_node_t *tree_delete_impl(tree_node_t *root, value_t key) {
  // 还是按照递归的顺序来查找key
  if (root == NULL) {
    return NULL;
  }

  if (key < root->key) {
    // 我们的返回值是什么??
    // 这句话是什么意思
    // 我们返回的是 在以root为根的子树中 删除key之后 剩下的子树的根
    // 所以这样递归的写 root的left肯定是对的
    root->left = tree_delete_impl(root->left, key);

    // 在这个地方插入fixup

  } else if (key > root->key) {
    root->right = tree_delete_impl(root->right, key);
  } else {
    // prev or next
    // next
    // 分三种情况
    // 第一种情况是左右孩子都是null
    // 直接返回NULL
    // 第二种情况是只有一个孩子
    // 如果只有左孩子 那么就应该返回左孩子
    // 如果只有右孩子 那么就应该返回右孩子
    // 上面三种情况可以总结成两句代码
    // 不对，这个实现是没有parent的实现
    // 这三种情况下 都需要释放掉root
    if (root->left == NULL) {
      // 在这个地方需要释放掉节点
      tree_node_t *new_root = root->right;
      free(root);
      return new_root;
    }
    if (root->right == NULL) {
      tree_node_t *new_root = root->left;
      free(root);
      return new_root;
    }

    // case 3: 两个孩子
    // 分成两种对称的情况
    if (root->left->height < root->right->height) {
      // 找到后继
      tree_node_t *next = tree_min_impl(root->right);
      next->left = root->left;
      // 在delete_min里面处理case 3.1 3.2的两种特殊情况
      next->right = tree_delete_min_impl(root->right);

      // 让后继作为新的根返回
      // 那么旧的根就需要释放掉
      free(root);
      root = next;
    } else {
      // 找到前驱
      tree_node_t *prev = tree_max_impl(root->left);
      prev->right = root->right;
      prev->left = tree_delete_max_impl(root->left);
      // 让前驱作为新的根
      // 旧的根释放掉
      free(root);
      root = prev;
    }
  }

  // 更新树的高度
  tree_update_height(root);
  root = avl_balance(root);
  return root;
}

// 然后实现delete
void avl_delete(avl_t *avl, value_t key) {
  assert(avl);
  if (avl_is_empty(avl)) {
    return;
  } else {
    // 我希望这可以包括删除根的情况
    // 这确实可以包括删除根的情况 但是没有正确的释放资源
    avl->root = tree_delete_impl(avl->root, key);
  }
}

// check_avl

typedef void (*tree_handle)(value_t key);

static void tree_foreach_inorder_impl(tree_node_t *root, tree_handle handle) {
  if (root) {
    tree_foreach_inorder_impl(root->left, handle);
    handle(root->key);
    tree_foreach_inorder_impl(root->right, handle);
  }
}

// 那么我需要一个全局的vector
// 那这样我就需要定义一个全局数组
// 在单线程的代码中 这样做是可以的
static vector_t vector = {NULL, 0, 0};
void avl_handle(value_t key) { vector_push_back(&vector, key); }

static void check_order(tree_node_t *root) {
  vector_clear(&vector);
  // 以root为根的子树的左节点 根 右节点必须保证顺序
  // 如果有的话
  // 我忘了 这样检查是不对的
  // 用中序遍历生成一个vector 然后检查顺序
  tree_foreach_inorder_impl(root, avl_handle);
  // 检查数组的顺序
  // check_order();
  check_array_order(vector.data, vector.size);
  // for (size_t i = 0; i < vector.size; ++i) {
  //   printf("%d\n", vector.data[i]);
  // }
}

// 先不实现平衡操作
// 先把balance树给测过了再写
// 毕竟相当于重写了一部分代码
void check_avl(avl_t *avl) {
  assert(avl);
  // check 分成三个部分
  // 第1部分是检查顺序
  check_order(avl->root);
  // 第二部分是检查平衡条件
  check_balance(avl->root);
}