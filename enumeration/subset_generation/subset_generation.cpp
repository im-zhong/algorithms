// 2024/10/25
// zhangzhong

#include <iostream>
#include <vector>

// 先写最简单的方式，就是需要生成的集合是数字，这样不需要额外提供一个集合数组
// 增量构造：就是不断向答案A中定序的扔新的元素。
void PrintSubsetV1(int n, int* A, int cur) {

    // 和枚举不同的是，我们每次递归都需要输出子集，而不用等全部排列完毕
    for (int i = 0; i < cur; i++) {
        // 而当前的子集数量就是cur
        // 所以最开始的时候cur是0 就代表我们的子集是空集

        // 但是空集要怎么输出呢?
        std::cout << A[i] << ",";
    }
    std::cout << "\n";

    // 获取下一个要加入的集合的元素
    // 这里使用定序的trick
    // 我们永远不会加入比我们当前集合中元素小的元素
    int s = 0;
    if (cur != 0) {
        // cur代表当前要写入的元素，所以在这里是不能访问的
        // cur - 1表示当前集合中额最大元素
        // 这个条件由定序来保证
        s = A[cur - 1] + 1;
    }
    // int s = cur ? A[cur - 1] + 1 : 0;
    // 我写的语句和书上的是等价的

    for (; s < n; s++) {
        // 将下一个元素s写入当前位置cur
        A[cur] = s;
        // 然后递归打印之后的子集
        PrintSubsetV1(n, A, cur + 1);
    }
}

// 位向量法：递归的构造位向量，然后根据位向量映射到元素上构造子集
void PrintSubsetV2(int n, int* A, int cur) {

    if (cur == n) {
        for (int i = 0; i < n; i++) {
            if (A[i] == 1) {
                std::cout << i << ",";
            }
        }
        std::cout << "\n";
    } else {

        // 这种构造位向量的方式说实话有点刻意了
        // 我们可以直接枚举数字啊
        A[cur] = 0;
        PrintSubsetV2(n, A, cur + 1);
        A[cur] = 1;
        PrintSubsetV2(n, A, cur + 1);
    }
}

// bitset
// 这是最简单的方法，我非常喜欢！
// 先写一个最简单的函数，就是根据元素数量和一个整数表示的集合，输出元素
void PrintSubset(int n, int s) {
    for (int i = 0; i < n; i++) {
        if (s & (1 << i)) {
            std::cout << i << ",";
        }
    }
    std::cout << "\n";
}

void PrintSubsetV3(int n) {

    // 现在我们只需要枚举整数就ok啦
    for (int s = 0; s < (1 << n); s++) {
        PrintSubset(n, s);
    }
}

int main() {
    // 2^3 = 8
    // 而我们算上空集和自己刚好也是8个，完美！
    int A[3] = {};
    PrintSubsetV1(3, A, 0);

    PrintSubsetV2(3, A, 0);

    // 漂亮！
    PrintSubsetV3(3);
}