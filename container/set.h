// 2022/11/19
// zhangzhong
// set

#ifndef __SET_H__
#define __SET_H__

// 定义set interface

#include "list.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef int64_t value_t;

// 接口就模仿cpp std::set<int> 就足够了
typedef struct {
  list_node_t head;
  size_t size;
} set_t;

typedef struct __set_entry_t {
  value_t value;
  list_node_t node;
} set_entry_t;

bool set_search(set_t *set, value_t value);
void set_insert(set_t *set, value_t value);
void set_delete(set_t *set, value_t value);
bool set_is_empty(set_t *set);
void set_clean(set_t *set);

set_t make_set();
void free_set(set_t *set);

#endif // __SET_H__
