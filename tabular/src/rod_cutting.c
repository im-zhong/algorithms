// 2023/3/13
// zhangzhong
// dynamic programing: rod cutting

#include "tabular/rod_cutting.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static int min(int x, int y) { return x < y ? x : y; }
static int max(int x, int y) { return x > y ? x : y; }

static void check(const int* prices, int size, int rod_len) {
    assert(rod_len > 0);
    assert(prices != NULL);
    // 所有的价格都要 > 0 除了prices[0] == 0
    assert(size > 0);
    assert(prices[0] == 0);
    for (int i = 1; i < size; i++) {
        assert(prices[i] > 0);
    }
}

// todo
static void print_solution(const int* cuts, int rod_len) {
    int remain = rod_len;
    printf("%d: ", rod_len);
    while (remain > 0) {
        int cut = cuts[remain];
        printf("%d, ", cut);
        remain -= cut;
    }
    printf("\n");
}

int rod_cutting(const int* prices, int size, int rod_len) {
    check(prices, size, rod_len);

    // 1. 递推式
    // rod(n) = max{i = 1...n : prices[i] + rod(n - i)}
    // 2. 计算顺序 1 -> n
    // if (rod_len == 0) {
    //     return 0;
    // }

    // 数组长度需要是 rod_len + 1, 因为 0 是无用的 用来存放0
    //
    int revenues[rod_len + 1];
    memset(revenues, 0, (rod_len + 1) * sizeof(int));
    int cuts[rod_len + 1];
    memset(cuts, 0, (rod_len + 1) * sizeof(int));

    // 从0开始是没有意义的 因为最短的长度都是1
    for (int i = 1; i <= rod_len; i++) {
        // 这一步就是计算递推式
        int revenue = 0;
        int cut = 0;
        // i表示的就是当前考虑的杆的长度
        for (int j = 1; j <= i; j++) {
            // 我们还得记录最大的划分的位置
            // r = min(r, prices[j] + revenue[i - j]);
            int curr_revenue = prices[j] + revenues[i - j];
            if (curr_revenue > revenue) {
                revenue = curr_revenue;
                cut = j;
            }
        }
        revenues[i] = revenue;
        cuts[i] = cut;
    }

    // 最终返回的就是 revenues[rod_len]
    print_solution(cuts, rod_len);
    return revenues[rod_len];
    // 还有一个需要考虑的问题是我们是如何cut的
    // 或者我们可以输出这个序列
    // 返回太麻烦了 我们还是输出吧
}
