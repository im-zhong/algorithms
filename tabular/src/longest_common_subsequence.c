// 2023/3/13
// zhangzhong
// dynamic programming: longest common subsequence

#include "tabular/longest_common_subsequence.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int min(int x, int y) { return x < y ? x : y; }
static int max(int x, int y) { return x > y ? x : y; }

struct array_t {
    int* data;
    size_t row;
    size_t col;
};

static struct array_t make_array(size_t row, size_t col, int initial) {
    int* data = malloc(row * col * sizeof(int));
    memset(data, initial, row * col * sizeof(int));
    struct array_t array = {
        data,
        row,
        col,
    };
    return array;
}

static void free_array(struct array_t* array) {
    if (array) {
        free(array->data);
        array->data = NULL;
        array->row = 0;
        array->col = 0;
    }
}

static int* at(const struct array_t* array, size_t i, size_t j) {
    return array->data + i * array->col + j;
}

// 定义三个方向
// 1 = left up
// 2 = left
// 3 = up

enum Direction {
    LEFT_UP = 1,
    LEFT,
    UP,
};

void print_lcs(const struct array_t* direction, const char* x, const char* y,
               size_t i, size_t j) {
    int d = *at(direction, i, j);
    if (d == LEFT_UP) {
        // 向坐上移动
        print_lcs(direction, x, y, i - 1, j - 1);
        assert(x[i - 1] == y[j - 1]);
        printf("%c", x[i - 1]);
    } else if (d == LEFT) {
        print_lcs(direction, x, y, i, j - 1);
    } else if (d == UP) {
        print_lcs(direction, x, y, i - 1, j);
    } else if (d == 0) {
        // 结束了
        return;
    } else {
        // bug
        assert(false);
    }
}

void print_solution(const struct array_t* direction, const char* x,
                    const char* y) {
    size_t row = strlen(x);
    size_t col = strlen(y);

    // 从 direction(x, y) 开始
    // 只要碰到 LEFT_UP 就把对应的字符放到栈里
    // 最后逆序输出即可
    // 那么用递归是最方便的方法

    print_lcs(direction, x, y, row, col);
    printf("\n");
}

size_t longest_common_subsequence(const char* x, const char* y) {

    // 第一步: 递推式
    // 因为我们的下标时从零开始的 这里就涉及到一个很丑陋的转换
    // 要么我们让x y的访问从下标1开始，要么我么让二维数组的下标访问从-1开始
    // 因为在真正处理第一行的时候，cost
    // 真的是这样吗？我们自己跑一个实际的例子试一下 看一看能不能直接求出
    // 解的第一行和第一列 这样就可以回避这个丑陋的实现了
    // lcs(i, j) =
    // 1. i == -1, j == -1
    // 二维数组的下标还是只能从零开始的
    // 只不过我们现在必须要多分配一行一列
    size_t row = strlen(x);
    size_t col = strlen(y);
    struct array_t lcs = make_array(row + 1, col + 1, 0);
    struct array_t direction = make_array(row + 1, col + 1, 0);

    // 计算顺序 按照行序进行计算即可
    // 只不过下标从1开始
    // 只需要在访问lcs的时候 自动给所有下标加上1就可以了
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            if (x[i] == y[j]) {
                // 还需要记录方向
                // 这样我们后面可以输出这个lcs
                // 这里的长度需要加上1 因为我们已经找到了两个相同的符号
                *at(&lcs, i + 1, j + 1) = *at(&lcs, i, j) + 1;
                *at(&direction, i + 1, j + 1) = LEFT_UP;
            } else {
                // 向左
                int left = *at(&lcs, i + 1, j);
                // 向上
                int up = *at(&lcs, i, j + 1);
                // 我们看看谁长
                if (left > up) {
                    // 选择left
                    *at(&direction, i + 1, j + 1) = LEFT;
                } else {
                    // 选择up
                    *at(&direction, i + 1, j + 1) = UP;
                }
                *at(&lcs, i + 1, j + 1) = max(left, up);
            }
        }
    }

    int len = *at(&lcs, row, col);
    print_solution(&direction, x, y);
    free_array(&lcs);
    free_array(&direction);
    return len;
}
