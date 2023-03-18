// 2023/3/18
// zhangzhong
// test suffix expression evaluation

#include "container/list.h"
#include "container/queue.h"
#include "expr/expr.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void expr_push(list_node_t* expr, const char* str) {
    strlist_t* entry = malloc(sizeof(strlist_t));
    entry->str = strdup(str);
    list_insert_before(expr, &entry->node);
}

int main() {
    // ((1 + 2) * 3 + 4) * 5 = 65
    list_node_t expr;
    list_init_head(&expr);
    expr_push(&expr, "(");
    expr_push(&expr, "(");
    expr_push(&expr, "1");
    expr_push(&expr, "+");
    expr_push(&expr, "2");
    expr_push(&expr, ")");
    expr_push(&expr, "*");
    expr_push(&expr, "3");
    expr_push(&expr, "+");
    expr_push(&expr, "4");
    expr_push(&expr, ")");
    expr_push(&expr, "*");
    expr_push(&expr, "5");

    value_t result = eval_infix_expr(&expr);
    assert(result == 65);
    return 0;
}
