// 2022/11/19
// zhangzhong
// AVL tree

#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <assert.h>
#include <container/def.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define max(x, y) ((x) > (y) ? (x) : (y))

// 自底向上两趟的方法是实现起来比较简单的
// 但是性能较差
// 主要是一些操作都是通用的
// 如果可以的话 联系起来更好
// typedef int64_t value_t;

// 普通的二叉树 AVL树 rbtree都可以使用这个作为结点类型 通用性更好
// 很多底层的操作都是一样的
// todo: 目前就不实现了吧 毕竟考试也不考 实现起来还是有一些麻烦的
typedef struct tree_node_t {
  struct tree_node_t *left;
  struct tree_node_t *right;
  value_t key;
  // 这个height说的是他的子树的高度
  size_t height;
} tree_node_t;

typedef struct {
  tree_node_t *root;
} avl_t;

avl_t make_avl();
void free_avl(avl_t *avl);
void check_avl(avl_t *avl);

bool avl_is_empty(avl_t *avl);
void avl_clean(avl_t *avl);
bool avl_search(avl_t *avl, value_t key);
void avl_insert(avl_t *avl, value_t key);
void avl_delete(avl_t *avl, value_t key);

#endif // __AVL_TREE__
