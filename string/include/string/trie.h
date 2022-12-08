// 2022/12/8
// zhangzhong
// trie 单词查找树

#ifndef __TRIE__
#define __TRIE__

#include "util/c_string.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef int64_t value_t;
#define ASCII_SIZE 256

// 单词查找树是键值对 key - value
typedef struct trie_node_t {
  // 那么我们怎么知道这个节点有没有值呢??
  // 所以还是需要一个标志位
  struct trie_node_t *child[ASCII_SIZE];
  value_t value;
  bool has_value;
} trie_node_t;

typedef struct {
  trie_node_t *root;
} trie_t;

void make_trie(trie_t *trie);
void free_trie(trie_t *trie);

void trie_clear(trie_t *trie);

bool trie_search(trie_t *trie, char *str, value_t *value);
void trie_insert(trie_t *trie, char *str, value_t value);
void trie_delete(trie_t *trie, char *str);

// 单词查找树含有key str吗
// bool trie_contain(trie_t *trie, const char *str);

// 单词查找树是空的吗
bool trie_is_empty(trie_t *trie);

// 有了dfs 这三个都非常好实现 就不实现了
// 单词查找树中的key
// 并且这个key是str的前缀
// 最长的
// char *trie_longest_prefix_of(trie_t *trie, const char *str);
// char **trie_keys_with_prefix(trie_t *trie, const char *str);
// char **trie_keys_that_match(trie_t *trie, const char *str);

// 正序遍历trie树
typedef void (*trie_handle)(string_t *str);

void trie_foreach(trie_t *trie, trie_handle fn);

// 当然也可以倒序遍历 虽然没啥必要

// 范围遍历
// 这个没想到怎么实现
void trie_range_foreach(trie_t *trie, char *floor, char *ceiling,
                        trie_handle fn);

#endif // __TRIE__
