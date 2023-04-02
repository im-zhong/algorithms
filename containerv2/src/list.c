// 2023/4/2
// zhangzhong
// list

#include "container/list.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// TODO(zhangzhong): 把list_node_t的声明与相关实现函数分开放到两个头文件里面
// TODO(zhangzhong): 排序链表的merge操作
// TODO(zhangzhong): 以链表表示集合的相关集合操作，交集 并集 差集
// TODO(zhangzhong): reverse操作反转链表
// TODO(zhangzhong): 排序链表的unique操作，这些好像都是stl里面的算法

list_node_t* make_list() {
    list_node_t* head = malloc(sizeof(list_node_t));
    assert(head != NULL);
    head->prev = head;
    head->next = head;
    return head;
}

// 释放head指向的链表 但是head本身不会被释放掉
void free_list(list_node_t* head, void (*free_entry)(void* entry)) {}

static inline void list_insert_between_impl(list_node_t* prev,
                                            list_node_t* next,
                                            list_node_t* node) {
    prev->next = node;
    node->prev = prev;
    next->prev = node;
    node->next = next;
}

void list_insert_after(list_node_t* node, void* entry) {
    list_insert_between_impl(node, node->next, entry);
}

void list_insert_before(list_node_t* node, void* entry) {
    list_insert_between_impl(node->prev, node, entry);
}

// constraint: prev != next
static inline void list_erase_between(list_node_t* prev, list_node_t* next) {
    prev->next = next;
    next->prev = prev;
}

void list_erase(list_node_t* head, void* entry) {
    list_node_t* node = entry;
    if (head == node) {
        return;
    }
    list_erase_between(node->prev, node->next);
}

bool list_is_head(list_node_t* head, void* entry) {
    return head == (list_node_t*)entry;
}

void* list_front(list_node_t* head) { return head->next; }
void* list_back(list_node_t* head) { return head->prev; }

void* list_next_entry(void* entry) { return ((list_node_t*)entry)->next; }

void* list_prev_entry(void* entry) { return ((list_node_t*)entry)->prev; }

bool list_empty(list_node_t* head) { return head->next == head; }

void list_push_front(list_node_t* head, void* entry) {
    list_insert_after(head, entry);
}
void list_push_back(list_node_t* head, void* entry) {
    list_insert_before(head, entry);
}
void list_pop_front(list_node_t* head) { list_erase(head, head->next); }
void list_pop_back(list_node_t* head) { list_erase(head->prev, head); }
