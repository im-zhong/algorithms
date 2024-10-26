// 2024/10/25
// zhangzhong

#include <iostream>

// 其实现在该去吃饭了
// 今天晚上要完成八皇后问题 + 三道例题
// 明天完成本章节剩下的三道例题
// 然后对目前学到的所有知识做一个整理
// 开始下一个阶段的学习

// 本质上所有的枚举问题，出了可以直接枚举的一些简单题目
// 大多数都是回溯，也就是递归 剪枝 枚举
// 递归的本质是不断的缩小问题的规模
// 比如在排列问题中，我们每次递归，都会把n个元素的排列问题变成n-1个元素的排列问题
// 同样，在八皇后问题中，我们也可以把八皇后问题 变成七皇后问题，六皇后问题
// 同时，每次递归，都会对我们的子问题产生约束，而约束产生剪枝，知道递归无法继续下去（回溯），或者该问题产生了一个解

// 而在该问题中，我们需要知道的约束条件就是，我们上一行的皇后被放在了那里
// 这个条件可以用一个数组进行描述
int tot = 0;
void EightQueen(int cur_row, int* queens, int queen_count) {

    // 递归结束条件就是我们把所有queues都放在了一个合适的位置
    // 也就是cur = queue_count
    if (cur_row == queen_count) {
        // 我们打印所有皇后的位置
        for (int i = 0; i < queen_count; i++) {
            std::cout << queens[i] << ",";
        }
        std::cout << "\n";
        tot += 1;
        return;
    }

    // 否则的话，我们就要查看约束条件
    // 首先不能同行，这个由cur保证，每次递归调用cur都会+1,表示我们将要在下一行放置新的皇后
    // 然后不能同列。这个可以在循环中进行保证
    // 最后是不能同对角线，可能八皇后问题唯一难写的地方就是怎么检测对角线吧
    // 只要仍然满足条件，我们就继续递归，否则就不递归了
    // 就这么简单
    for (int col = 0; col < queen_count; col++) {
        // 检查是否与已经放置的同列
        bool is_on_same_column = false;
        bool is_on_same_tri = false;
        for (int row = 0; row < cur_row; row++) {
            if (queens[row] == col) {
                is_on_same_column = true;
                break;
            }
            // 检查是否在主对角线上
            // 其实也没有什么,mnxz难的
            // 我们只需要检查某行的queue在当前列的主对角线位置
            // 也就是 banned_col = queens[row] + (cur_row-row)
            // 负对角线 banned_col = queens[row] - (cur_row-row)
            if (col == queens[row] + (cur_row - row) ||
                col == queens[row] - (cur_row - row)) {
                is_on_same_tri = true;
                break;
            }
        }
        if (is_on_same_column || is_on_same_tri) {
            continue;
        }

        // 如果没有continue 说明当前位置是合法的
        // 我们就缩小问题规模 继续递归
        // 不好意思，又忘了把皇后给放下了
        queens[cur_row] = col;
        EightQueen(cur_row + 1, queens, queen_count);
    }
}

int main() {
    int queens[8] = {};
    EightQueen(0, queens, 8);
    // 92, which is correct!
    std::cout << tot << std::endl;
}