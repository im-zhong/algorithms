// 2022/12/6
// zhangzhong
// skiplist

#ifndef __SKIPLIST__
#define __SKIPLIST__

#include "util/c_random.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef int64_t value_t;

// 数据量越大 这个level应该越大
#define MAX_LEVEL 8
// 概率是 1/2 就折半查找

// 我们需要一个链表节点
typedef struct skipnode_t {
    value_t value;
    struct skipnode_t* right;
    struct skipnode_t* down;
} skipnode_t;

// 链表的那些操作自己写的 反正也很简单

// 然后定义一个skiplist_t
typedef struct {
    skipnode_t* header;
    skipnode_t bottom;
    skipnode_t tail;
    int level;
    rand_t r;
} skiplist_t;

skipnode_t* make_skipnode(skiplist_t* sl, value_t value);

void make_skiplist(skiplist_t* sl);
void free_skiplist(skiplist_t* sl);

// 算了 还是不想的复杂了 就实现一个简单版本的skiplist吧
// 提供三个简单的操作
void skiplist_insert(skiplist_t* sl, value_t value);
bool skiplist_search(skiplist_t* sl, value_t value);
void skiplist_delete(skiplist_t* sl, value_t value);

// 那么问题来了 我们怎么检查skiplist的结构呢
void check_skiplist(skiplist_t* sl);

#endif // __SKIPLIST__
