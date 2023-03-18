// 2023/3/18
// zhangzhong
// expr eval

#ifndef EXPR_EVAL_H
#define EXPR_EVAL_H

#include "container/def.h"
#include "container/list.h"
#include "container/stack.h"

// 我们的输入是一个字符串的链表
// 我们的输出是其值

typedef struct {
    list_node_t node;
    char* str;
} strlist_t;

value_t eval_infix_expr(list_node_t* expr);

value_t eval_suffix_expr(list_node_t* expr);

#endif // EXPR_EVAL_H
