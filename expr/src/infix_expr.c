// 2023/3/18
// zhangzhong
// infix expression evaluation

#include "container/list.h"
#include "container/slist.h"
#include "container/stack.h"
#include "expr/expr.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    int type;
    value_t value;
    INHERIT_STACK;
} expr_t;

// 我们需要检查是不是左括号 右括号 操作符呀
// 我们可以给每个符号一个数字 同时用他们的大小代表优先级
// 我们还得定义一个特殊的符号 结束符 并且他的优先级是最低的
enum {
    EXPR_END = 0,
    EXPR_LBRACKET = 1,
    EXPR_RBRACKET = 2,
    EXPR_OP_ADD = 3,
    EXPR_OP_SUB = 4,
    EXPR_OP_MUL = 5,
    EXPR_OP_DIV = 6,
    EXPR_NUM = 7,
};

static int get_expr_type(const char* str) {
    if (str[0] == '(') {
        return EXPR_LBRACKET;
    } else if (str[0] == ')') {
        return EXPR_RBRACKET;
    } else if (str[0] == '+') {
        return EXPR_OP_ADD;
    } else if (str[0] == '-') {
        return EXPR_OP_SUB;
    } else if (str[0] == '*') {
        return EXPR_OP_MUL;
    } else if (str[0] == '/') {
        return EXPR_OP_DIV;
    } else {
        return EXPR_NUM;
    }
}

static value_t str_to_value(const char* str) {
    errno = 0;
    value_t value = strtol(str, NULL, 10);
    assert(errno == 0);
    return value;
}

static value_t eval(int op, value_t lhs, value_t rhs) {
    switch (op) {
    case EXPR_OP_ADD:
        return lhs + rhs;
    case EXPR_OP_SUB:
        return lhs - rhs;
    case EXPR_OP_MUL:
        return lhs * rhs;
    case EXPR_OP_DIV:
        return lhs / rhs;
    default:
        assert(false);
    }
}

static void eval_top(slist_node_t* num_stack, slist_node_t* op_stack) {

    assert(!stack_is_empty(op_stack));
    expr_t* op = stack_top(op_stack, expr_t);
    stack_pop(op_stack);

    // 然后根据op的类型做操作
    if (op->type == EXPR_RBRACKET) {
        // (num)
        assert(!stack_is_empty(num_stack));
        expr_t* num = stack_top(num_stack, expr_t);
        stack_pop(num_stack);
        assert(num->type == EXPR_NUM);

        assert(!stack_is_empty(num_stack));
        expr_t* lbracket = stack_top(num_stack, expr_t);
        stack_pop(num_stack);
        assert(lbracket->type == EXPR_LBRACKET);

        // 将num压入num_stack
        stack_push(num_stack, num);
    } else {
        // 用枚举来表示类型 运算起来就麻烦了
        // 从栈顶pop出两个元素 然后计算
        // 然后把结果push进去
        assert(!stack_is_empty(num_stack));
        expr_t* rhs = stack_top(num_stack, expr_t);
        stack_pop(num_stack);

        assert(!stack_is_empty(num_stack));
        expr_t* lhs = stack_top(num_stack, expr_t);
        stack_pop(num_stack);

        value_t result = eval(op->type, lhs->value, rhs->value);
        expr_t* stack_entry = malloc(sizeof(expr_t));
        stack_entry->type = EXPR_NUM;
        stack_entry->value = result;
        stack_push(num_stack, stack_entry);
    }
}

int get_op_priority(int op) {
    switch (op) {
    case EXPR_END:
        return 0;
    case EXPR_RBRACKET:
        return 1;
    case EXPR_OP_ADD:
    case EXPR_OP_SUB:
        return 2;
    case EXPR_OP_MUL:
    case EXPR_OP_DIV:
        return 3;
    default:
        assert(false);
    }
}

bool need_eval_top(int top_op, int curr_op) {
    // 当 top >= curr 的时候 我们就需要eval top
    // 右括号的优先级需要额外考虑
    if (top_op >= curr_op) {
        return true;
    }
    // top_op < curr_op
    // 如果top是右括号 那么我们就一定需要计算
    return top_op == EXPR_RBRACKET;
}

value_t eval_infix_expr(list_node_t* expr) {

    // 我们需要两个栈
    // 一个用来存储操作数
    // 一个用来存储操作符

    stack_init(op_stack);
    stack_init(num_stack);

    // 从左到右扫描表达式
    // 如果遇到操作数，就将其压入操作数栈
    // 如果遇到操作符，就将其压入操作符栈
    // 如果遇到左括号，压入操作数栈
    // 遇到的操作符要根据优先级进行处理
    // 如果遇到右括号，就将操作符栈顶的操作符弹出
    // 并弹出操作数栈顶的两个操作数，进行运算
    // 将运算结果压入操作数栈
    // 重复上述过程，直到操作符栈为空

    // 我们要遍历这个链表
    strlist_t* entry = NULL;
    list_for_each_entry(entry, expr, strlist_t, node) {
        int op_type = get_expr_type(entry->str);
        switch (op_type) {
        case EXPR_LBRACKET: {
            expr_t* lbracket = malloc(sizeof(expr_t));
            lbracket->type = EXPR_LBRACKET;
            stack_push(&num_stack, lbracket);
            break;
        }
        case EXPR_NUM: {
            expr_t* num = malloc(sizeof(expr_t));
            num->type = EXPR_NUM;
            num->value = str_to_value(entry->str);
            stack_push(&num_stack, num);
            break;
        }
        default: {
            if (!stack_is_empty(&op_stack)) {
                // 拿到栈顶操作符
                expr_t* top_op = stack_top(&op_stack, expr_t);
                // 拿到当前操作符
                if (need_eval_top(top_op->type, op_type)) {
                    eval_top(&num_stack, &op_stack);
                }
            }
            // 然后再把我们push进去
            expr_t* op = malloc(sizeof(expr_t));
            op->type = op_type;
            stack_push(&op_stack, op);
            break;
        }
        }
    }

    // 最后eval top
    eval_top(&num_stack, &op_stack);

    // op stack 应该是空的
    assert(stack_is_empty(&op_stack));

    // 最后的结果存放再num stack中
    assert(!stack_is_empty(&num_stack));
    expr_t* result = stack_top(&num_stack, expr_t);
    stack_pop(&num_stack);
    assert(stack_is_empty(&num_stack));

    return result->value;
}
