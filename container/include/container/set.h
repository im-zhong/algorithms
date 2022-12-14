// 2022/11/19
// zhangzhong
// set

#ifndef _SET_H_
#define _SET_H_

// 定义set interface

#include "def.h"
#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

// 接口就模仿cpp std::set<int> 就足够了
typedef struct {
    list_node_t head;
    size_t size;
} set_t;

typedef struct {
    value_t value;
    list_node_t node;
} set_entry_t;

static inline set_entry_t* make_set_entry(value_t value) {
    set_entry_t* entry = (set_entry_t*)malloc(sizeof(set_entry_t));
    entry->value = value;
    return entry;
}

static inline set_entry_t* set_search_impl(set_t* set, value_t value) {
    assert(set);
    set_entry_t* entry = NULL;
    list_for_each_entry(entry, &set->head, set_entry_t, node) {
        if (value == entry->value) {
            return entry;
        }
    }
    return NULL;
}

static inline bool set_search(set_t* set, value_t value) {
    return set_search_impl(set, value) != NULL;
}

static inline void set_insert(set_t* set, value_t value) {
    assert(set);
    // 那么就得先search啊
    if (!set_search(set, value)) {
        set_entry_t* entry = make_set_entry(value);
        list_insert_after(&set->head, &entry->node);
        ++set->size;
    }
}

static inline void set_delete(set_t* set, value_t value) {
    set_entry_t* entry = set_search_impl(set, value);
    if (entry) {
        list_erase(&entry->node);
        --set->size;
        free(entry);
    }
}
static inline bool set_is_empty(set_t* set) { return set->size == 0; }
static inline void set_clean(set_t* set) {
    // free list
}

// 这个好像没法做到返回一个值啊 因为head必须要进行初始化
static inline void make_set(set_t* set) {
    assert(set);
    set->size = 0;
    list_init_head(&set->head);
}
static inline void free_set(set_t* set) {
    assert(set);
    // todo: free_list
}

#endif // _SET_H_
