// 2023/3/13
// zhanzhong

#include "tabular/matrix_chain_multiplication.h"
#include "tabular/rod_cutting.h"
#include <assert.h>
#include <stdlib.h>

void test_rod_cutting() {
    int prices[] = {
        0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30,
    };
    const int size = sizeof(prices) / sizeof(int);

    assert(rod_cutting(prices, size, 1) == 1);
    assert(rod_cutting(prices, size, 2) == 5);
    assert(rod_cutting(prices, size, 3) == 8);
    assert(rod_cutting(prices, size, 4) == 10);
    assert(rod_cutting(prices, size, 5) == 13);
    assert(rod_cutting(prices, size, 6) == 17);
    assert(rod_cutting(prices, size, 7) == 18);
    assert(rod_cutting(prices, size, 8) == 22);
    assert(rod_cutting(prices, size, 9) == 25);
    assert(rod_cutting(prices, size, 10) == 30);
}

void test_matrix_chain_multiplication() {
    // test case
    //   A0    A1   A2   A3    A4   A5
    // 30x35 35x15 15x5 5x10 10x20 20x25
    int matrix[] = {
        30, 35, 15, 5, 10, 20, 25,
    };
    int size = sizeof(matrix) / sizeof(int);
    assert(matrix_chain_multiplication(matrix, size) == 15125);

    // 可以测一个小一点的例子
    int matrix2[] = {
        30,
        35,
        15,
    };
    int size2 = sizeof(matrix2) / sizeof(int);
    assert(matrix_chain_multiplication(matrix2, size2) == 30 * 35 * 15);
}

int main() {
    test_rod_cutting();
    test_matrix_chain_multiplication();
    return 0;
}
