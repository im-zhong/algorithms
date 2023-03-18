// 2023/3/18
// zhangzhong
// suffix expression evaluation

#include "container/def.h"
#include "container/list.h"
#include "container/stack.h"
#include "expr/expr.h"
#include "util/c_string.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

// enum {
//     EXPR_OP,
//     EXPR_NUM,
// };

// 这些个玩意用起来真是麻烦啊
// 最好可以统一一下使用方法
// 在多个源文件里面写的类型会冲突吗 ??
typedef struct {
    // EXPR_OP or EXPR_NUM
    // 不对呀 op根本就不会被push进去啊
    // int type;
    // '+' '-' '*' '/' or number
    value_t value;
    INHERIT_STACK;
} expr_t;

static bool is_op(const char* str) {
    return str[0] == '+' || str[0] == '-' || str[0] == '*' || str[0] == '/';
}

static value_t str_to_value(const char* str) {
    errno = 0;
    value_t value = strtol(str, NULL, 10);
    assert(errno == 0);
    return value;
}

static value_t eval(char op, value_t lhs, value_t rhs) {
    switch (op) {
    case '+':
        return lhs + rhs;
    case '-':
        return lhs - rhs;
    case '*':
        return lhs * rhs;
    case '/':
        return lhs / rhs;
    default:
        assert(false);
    }
}

value_t suffix_expr_eval(list_node_t* expr) {
    stack_init(stack);

    // 我们要遍历这个链表
    strlist_t* entry = NULL;
    list_for_each_entry(entry, expr, strlist_t, node) {
        if (is_op(entry->str)) {
            // 从栈顶pop出两个元素 然后计算
            // 然后把结果push进去
            assert(!stack_is_empty(&stack));
            expr_t* rhs = stack_top(&stack, expr_t);
            stack_pop(&stack);

            assert(!stack_is_empty(&stack));
            expr_t* lhs = stack_top(&stack, expr_t);
            stack_pop(&stack);

            value_t result = eval(entry->str[0], lhs->value, rhs->value);
            expr_t* stack_entry = malloc(sizeof(expr_t));
            stack_entry->value = result;
            stack_push(&stack, stack_entry);
        } else {
            // 如果是数字 直接push进去
            expr_t* stack_entry = malloc(sizeof(expr_t));
            // stack_entry->type = EXPR_NUM;
            stack_entry->value = str_to_value(entry->str);
            stack_push(&stack, stack_entry);
        }
    }

    // 最后栈里面只剩下一个元素
    // 这个元素就是结果
    assert(!stack_is_empty(&stack));
    expr_t* result = stack_top(&stack, expr_t);
    stack_pop(&stack);
    assert(stack_is_empty(&stack));

    // 为什么没有free stack??
    return result->value;
}
