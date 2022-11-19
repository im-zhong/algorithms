// 2022/11/19
// zhangzhong
// AVL tree

#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// 自底向上两趟的方法是实现起来比较简单的
// 但是性能较差
// 主要是一些操作都是通用的
// 如果可以的话 联系起来更好
typedef int64_t key_t;

enum color { red, black };

// 普通的二叉树 AVL树 rbtree都可以使用这个作为结点类型 通用性更好
// 很多底层的操作都是一样的
// todo: 目前就不实现了吧 毕竟考试也不考 实现起来还是有一些麻烦的
typedef struct tree_node_t {
  struct tree_node_t *parent;
  struct tree_node_t *left;
  struct tree_node_t *right;
  key_t key;
  size_t height;
  color color;
} tree_node_t;

typedef struct {
  tree_node_t *root;
} avltree_t;

#endif // __AVL_TREE__