// 2023/3/13
// zhangzhong
// dynamic programming: matrix chain multiplication

#include "tabular/matrix_chain_multiplication.h"
#include "container/array2d.h"
#include <assert.h>
#include <limits.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int matrix_multiplication_cost(const int* matrix, size_t n, size_t i,
                                      size_t j, size_t k) {
    // (Ai ... Ak) (Ak+1 ... Aj)
    // 这两组矩阵相乘所需的cost
    // 我们只需要找到Ai 所对应的行和列在哪??
    // Ai m[i], m[i+1]
    assert(i < n);
    assert(j < n);
    assert(k < n);
    return matrix[i] * matrix[k + 1] * matrix[j + 1];
}

// 二维数组
// struct array_t {
//     int* data;
//     size_t row;
//     size_t col;
// };

// struct array_t make_array(size_t row, size_t col, int initial) {
//     int* data = malloc(row * col * sizeof(int));
//     memset(data, initial, row * col * sizeof(int));
//     struct array_t array = {
//         data,
//         row,
//         col,
//     };
//     return array;
// }

// void free_array(struct array_t* array) {
//     if (array) {
//         free(array->data);
//         array->data = NULL;
//         array->row = 0;
//         array->col = 0;
//     }
// }

// int* at(struct array_t* array, size_t i, size_t j) {
//     return array->data + i * array->row + j;
// }

// 模拟一个二维数组
// int* cost(int i, int j) {
//     // 这实际上就是一个数组
//     // 二维数组的编程确实是一个不太好处理的问题
//     // 因为其大小往往取决于输入 所以我们只能通过
//     // 拿到一个一维数组的指针，然后用它模拟二位数组
//     // 那么一个好的访问模式 或者说 API 可以简化思考模型
//     // 使用起来体验也更好
//     // 在这里摸索一下
//     // a(i, j) =
//     // int* array = matrix(row * col * sizeof(int));
//     // array + i * row + j
//     // struct {int*, int row, int col}
//     // struct.at(i, j)
// }

static void check(const int* matrix, size_t size) {
    assert(matrix != NULL);
    assert(size > 0);
}

// 输出序列 (Ai ... Aj)
// 当且仅当 j - i > 1时 才会输出括号
static void print_impl(struct array2d_t* cuts, size_t i, size_t j) {
    assert(i <= j);
    if (i == j) {
        printf("A%zu", i);
    }
    // else if (j - i == 1) {
    //     printf("(A%zuA%zu)", i, j);
    // }
    else {
        int cut = *at(cuts, i, j);
        printf("(");
        print_impl(cuts, i, cut);
        print_impl(cuts, cut + 1, j);
        printf(")");
    }
}

static void print_solution(struct array2d_t* cuts) {
    size_t n = cuts->row;

    // 序列 A0 A1 A2 ... An-1
    // 括号化序列 (A0A1)(A2A3)
    print_impl(cuts, 0, n - 1);
    printf("\n");
}

//
int matrix_chain_multiplication(int* matrix, size_t size) {
    check(matrix, size);
    // 1. 找递推式
    // cost(Ai..j) = cost(Ai..k) + cost(Ak+1..j) + 前面两个矩阵最终乘起来
    // k: min(i -> j)
    // 我发现我需要一个计算cost的辅助函数

    // 2. 计算顺序
    // 按照step计算 00 11 22 ...
    // 01 12 23 ...
    // 02 13 ...
    // ...
    // 0 n-1

    // 矩阵的个数是 n = size - 1
    size_t n = size - 1;
    struct array2d_t costs = make_array(n, n, 0);
    struct array2d_t cuts = make_array(n, n, 0);

    // step = 0 没有意义 不需要计算 因为都被初始化为零了
    // 这样修改下面就不依赖cost的默认值为零了
    for (int step = 1; step < n; step++) {
        // 这里面的循环条件稍显复杂
        // 每一次考虑的Ai..j是 i, i + step
        // 最终的i + step 应该是小于n的
        for (int i = 0; (i + step) < n; i++) {
            int j = i + step;
            int cost = INT_MAX;
            int cut = 0;

            // 把这个东西写成一个函数吧 使用起来更方便
            // 这一步的核心想法就是计算 cost(i, j)
            // cost(i, j) =
            for (int k = i; k < j; k++) {
                // k >= i
                // k + 1 <= j
                // 这都是安全的 所以这个式子是对的
                // int curr_cost = cost(i, k) + cost(k + 1, j) +
                //                 matrix_multiplication_cost(matrix, n, i, j,
                //                 k);
                int curr_cost =
                    *at(&costs, i, k) + *at(&costs, k + 1, j) +
                    matrix_multiplication_cost(matrix, size, i, j, k);
                // 这里的cost默认都是零
                // 但是默认是零是有用的 A00 这样的计算是依赖默认值的
                if (curr_cost < cost) {
                    cost = curr_cost;
                    cut = k;
                }
            }

            // 最终在这里计算出了 cost(i, j)
            *at(&costs, i, j) = cost;
            *at(&cuts, i, j) = cut;
        }
    }

    print_solution(&cuts);
    int cost = *at(&costs, 0, n - 1);
    free_array(&costs);
    return cost;
}