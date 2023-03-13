// 2023/3/13
// zhangzhong
// dynamic programming: matrix chain multiplication

#ifndef MATRIX_CHAIN_MULTIPLICATION
#define MATRIX_CHAIN_MULTIPLICATION

#include <stddef.h>

// ref: Introduction to Algorithm, Third Edition, 15.2
//
// ABCD, (AB)(CD)
// cost = (p x q) * (q x r) = (q x r), cost = pqr
// input: 数组 指出了矩阵链的行列数量
// A1 A2 A3 ... An
// A0 A1 A2 ... An-1 共 n个矩阵
// matrix[n + 1]
// A0: m[0] x m[1]
// A1: m[1] x m[2]
// ...
// An-1: m[n-1] x m[n]
// 所以总共有 n + 1个元素
// output:
// 矩阵乘法的最小代价，同时给出对应此代价的计算顺序
int matrix_chain_multiplication(int* matrix, size_t size);

#endif // MATRIX_CHAIN_MULTIPLICATION
