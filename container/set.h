// 2022/11/19
// zhangzhong
// set

#ifndef __SET_H__
#define __SET_H__

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

typedef struct __set_entry_t {
  value_t value;
  list_node_t node;
} set_entry_t;

set_entry_t *make_set_entry(value_t value) {
  set_entry_t *entry = (set_entry_t *)malloc(sizeof(set_entry_t));
  entry->value = value;
  return entry;
}

set_entry_t *set_search_impl(set_t *set, value_t value) {
  assert(set);
  set_entry_t *entry = NULL;
  list_for_each_entry(entry, &set->head, set_entry_t, node) {
    if (value == entry->value) {
      return entry;
    }
  }
  return NULL;
}

bool set_search(set_t *set, value_t value) {
  return set_search_impl(set, value) != NULL;
}

void set_insert(set_t *set, value_t value) {
  assert(set);
  // 那么就得先search啊
  if (!set_search(set, value)) {
    set_entry_t *entry = make_set_entry(value);
    list_insert_after(&set->head, &entry->node);
    ++set->size;
  }
}

void set_delete(set_t *set, value_t value) {
  set_entry_t *entry = set_search_impl(set, value);
  if (entry) {
    list_erase(&entry->node);
    --set->size;
    free(entry);
  }
}
bool set_is_empty(set_t *set) { return set->size == 0; }
void set_clean(set_t *set) {
  // free list
}

// 这个好像没法做到返回一个值啊 因为head必须要进行初始化
void make_set(set_t *set) {
  assert(set);
  set->size = 0;
  list_init_head(&set->head);
}
void free_set(set_t *set) {
  assert(set);
  // todo: free_list
}

#endif // __SET_H__
