// 2022/12/8
// zhangzhong
// trie tree

#include "string/trie.h"
#include "util/c_string.h"
#include <assert.h>
#include <stdlib.h>

static trie_node_t *make_trie_node() {
  trie_node_t *node = (trie_node_t *)malloc(sizeof(trie_node_t));
  for (size_t i = 0; i < ASCII_SIZE; i++) {
    node->child[i] = NULL;
  }
  node->value = 0;
  node->has_value = false;
  return node;
}

void make_trie(trie_t *trie) { trie->root = NULL; }

void free_trie(trie_t *trie) {}

void trie_clear(trie_t *trie) {}

bool trie_search(trie_t *trie, char *str, value_t *value) {
  assert(trie);

  // 从根目录开始找
  trie_node_t *work = trie->root;
  // 这里的pos可以去掉
  while (*str && work) {
    // 我们可以向下
    work = work->child[*str];
    str++;
  }

  // 此时work可能是空指针啊
  if (*str == '\0' && work && work->has_value) {
    // 说明我们找到了
    if (value) {
      *value = work->value;
    }
    return true;
  }
  return false;
}

void trie_insert(trie_t *trie, char *str, value_t value) {
  // 插入也非常简单
  // 和查找差不多 只不过查找在没有的时候
  // search会跳出循环 二insert会插入数据
  if (!trie->root) {
    // 创建root节点
    // 可以在一开始的时候就创建root节点 这样insert就不需要做检查了
    trie->root = make_trie_node();
  }

  // 我们让root节点一定有效 可以省略掉一些if判断
  trie_node_t *work = trie->root;
  while (*str) {
    if (work->child[*str] == NULL) {
      work->child[*str] = make_trie_node();
    }
    work = work->child[*str];
    str++;
  }
  // 给work赋值即可
  assert(work);
  work->value = value;
  work->has_value = true;
}

// todo 可以给每个节点设置一个child_size的变量 表示此节点的孩子指针数量
// 此变量需要在insert和delete的时候进行维护 这样has_child就不需要遍历数组了
static bool has_child(trie_node_t *node) {
  assert(node);
  for (size_t i = 0; i < ASCII_SIZE; i++) {
    if (node->child[i]) {
      return true;
    }
  }
  return false;
}

static trie_node_t *trie_delete_impl(trie_node_t *root, char *str) {
  // 递归结束条件
  // 字符串已经没有了 或者root指针为空
  // 不对  str == '\0' 表示我们已经找到了这个节点
  // 有两种情况应该停止递归 一种是字符串到头了 一种是root是空指针

  if (root == NULL) {
    // 如果root为空 那么在下面检查has_child就会出错
    // 所以root为空 应该返回null
    return NULL;
  } else if (*str == '\0') {
    // 现在我们已经找到了节点
    // 如果root非空 我们就要删掉它
    root->has_value = false;
  } else {
    // 这个必须一层一层的写
    root->child[*str] = trie_delete_impl(root->child[*str], str + 1);
  }

  // 如果我们还有孩子 我们就返回我们自己的指针
  // bug case
  // 考虑这样一种情况
  // insert(a)
  // delete(ab)
  // 我们再递归的查找b的节点失败之后，因为我们没有b
  // 递归回到a节点，此时a是没有孩子的 但是a是不能删掉的 因为a有值
  if (root->has_value || has_child(root)) {
    return root;
  } else {
    // 如果我们没有孩子了 我们就需要把自己释放掉
    // 然后返回空指针 这样上层的child指针就被正确的设置了
    free(root);
    return NULL;
  }
}

// 删除同样也非常简单
void trie_delete(trie_t *trie, char *str) {
  trie->root = trie_delete_impl(trie->root, str);
}

bool trie_is_empty(trie_t *trie) { return trie->root; }

// 遍历trie的所有字符串
// depth = -1 表示不需要检查深度
void dfs_impl(trie_node_t *root, string_t *str, int depth, trie_handle fn) {
  if (!root) {
    return;
  }
  // check depth
  if (depth == 0) {
    return;
  }
  if (depth < 0) {
    depth = 0;
  }

  // do something to str
  if (root->has_value) {
    fn(str);
  }

  // 遍历所有的数组
  for (int i = 0; i < ASCII_SIZE; i++) {
    if (root->child[i]) {
      string_pushback(str, i);
      dfs_impl(root->child[i], str, depth - 1, fn);
      string_popback(str);
    }
  }
}

void trie_foreach(trie_t *trie, trie_handle fn) {
  string_t str = make_string("");
  dfs_impl(trie->root, &str, -1, fn);
  free_string(&str);
}

// 有了顺序遍历，prefix相关的操作会非常简单的实现
// todo: 还差一种range操作