// 2023/4/2
// zhangzhong
// list

#ifndef CONTAINER_LIST_H
#define CONTAINER_LIST_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * @brief   get struct ptr from its member ptr
 * @ptr:    struct member's address
 * @type:   struct type
 * @member: struct member name
 */
#define container_of(ptr, type, member)                                        \
    (type*)((char*)ptr - offsetof(type, member))

typedef struct list_node_t {
    struct list_node_t* prev;
    struct list_node_t* next;
} list_node_t;

// TODO(zhangzhong): 把list_node_t的声明与相关实现函数分开放到两个头文件里面
// TODO(zhangzhong): 排序链表的merge操作
// TODO(zhangzhong): 以链表表示集合的相关集合操作，交集 并集 差集
// TODO(zhangzhong): reverse操作反转链表
// TODO(zhangzhong): 排序链表的unique操作，这些好像都是stl里面的算法

list_node_t* make_list();
void free_list(list_node_t* head, void (*free_entry)(void* entry));

void list_insert_before(list_node_t* node, void* entry);
void list_insert_after(list_node_t* node, void* entry);
void list_erase(list_node_t* head, void* entry);

void* list_front(list_node_t* head);
void* list_back(list_node_t* head);
void list_push_front(list_node_t* head, void* entry);
void list_push_back(list_node_t* head, void* entry);
void list_pop_front(list_node_t* head);
void list_pop_back(list_node_t* head);
bool list_empty(list_node_t* head);

bool list_is_head(list_node_t* head, void* entry);
void* list_next_entry(void* entry);
void* list_prev_entry(void* entry);

#define list_for_each(head, entry)                                             \
    for ((entry) = list_front(head); !list_is_head(head, entry);               \
         (entry) = list_next_entry(entry))

#define list_for_each_reverse(head, entry)                                     \
    for ((entry) = list_back(head); !list_is_head(head, entry);                \
         (entry) = list_prev_entry(entry))

#endif // CONTAINER_LIST_H
