// 2022/12/8setmake_strset_entry_entry_t
// zhangzhong
// string set

#ifndef __STRSET__
#define __STRSET__

#include "def.h"
#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// 接口就模仿cpp map<string, int> 就足够了
// 我们得是一个kv啊
typedef struct {
    list_node_t head;
    size_t size;
} strset_t;

typedef struct __strset_entry_t {
    char* str;
    value_t value;
    list_node_t node;
} strset_entry_t;

static inline strset_entry_t* make_strset_entry(char* str, value_t value) {
    strset_entry_t* entry = (strset_entry_t*)malloc(sizeof(strset_entry_t));
    // 我们需要复制str
    entry->str = (char*)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(entry->str, str);
    entry->value = value;
    return entry;
}

static void free_strset_entry(strset_entry_t* entry) {
    if (entry) {
        free(entry->str);
        free(entry);
    }
}

static inline strset_entry_t* strset_search_impl(strset_t* set, char* str) {
    assert(set);
    strset_entry_t* entry = NULL;
    list_for_each_entry(entry, &set->head, strset_entry_t, node) {
        if (strcmp(str, entry->str) == 0) {
            return entry;
        }
    }
    return NULL;
}

static inline bool strset_search(strset_t* set, char* str, value_t* value) {
    strset_entry_t* entry = strset_search_impl(set, str);
    if (entry) {
        if (value) {
            *value = entry->value;
        }
        return true;
    }
    return false;
}

static inline void strset_insert(strset_t* set, char* str, value_t value) {
    assert(set);
    // 那么就得先search啊
    strset_entry_t* entry = strset_search_impl(set, str);
    if (entry) {
        entry->value = value;
    } else {
        strset_entry_t* entry = make_strset_entry(str, value);
        list_insert_after(&set->head, &entry->node);
        ++set->size;
    }
}

static inline void strset_delete(strset_t* set, char* str) {
    strset_entry_t* entry = strset_search_impl(set, str);
    if (entry) {
        list_erase(&entry->node);
        --set->size;
        free_strset_entry(entry);
    }
}
static inline bool strset_is_empty(strset_t* set) { return set->size == 0; }
static inline void strset_clean(strset_t* set) {
    // free list
}

// 这个好像没法做到返回一个值啊 因为head必须要进行初始化
static inline void make_strset(strset_t* set) {
    assert(set);
    set->size = 0;
    list_init_head(&set->head);
}
static inline void free_strset(strset_t* set) {
    assert(set);
    // todo: free_list
}

#endif // __STRSET__