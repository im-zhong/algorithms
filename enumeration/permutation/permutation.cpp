// 2024/10/25
// zhangzhong

// generate 1~n permutaion
#include <chrono>
#include <cstdint>
#include <iostream>
#include <set>
#include <vector>

// anonymous namespace
// https://clang.llvm.org/extra/clang-tidy/checks/misc/use-internal-linkage.html
// 我认为这个是不应该添加的，一切与程序真正目的无关的额外优化表述都应该在项目的最后考虑
namespace {
std::vector<uint64_t> GenerateRange(const uint64_t begin, const uint64_t end) {
    auto range = std::vector<uint64_t>{};
    for (uint64_t n = begin; n < end; n++) {
        range.push_back(n);
    }
    return range;
}

void PrintPermutation(const std::vector<uint64_t>& prefix,
                      const std::set<uint64_t>& remains) {
    // if there is no remains
    // then we should print the whole prefix as a permutation
    if (remains.empty()) {
        // for (uint64_t n : prefix) {
        //     std::cout << n << ", ";
        // }
        // std::cout << "\n";

        return;
    }

    // 这里直接写出具体的类型比写一个auto好多了
    // 不过在C++中如何禁止隐士类型转换呢？
    // 但是另外的问题是，如果我们修改了remains的类型，auto可以自动的修改类型
    // 但是具体的类型会导致潜在的bug，这都是由于C++的隐式类型转换导致的
    for (uint64_t n : remains) {
        // add it to a prefix
        auto new_prefix = prefix;
        new_prefix.push_back(n);

        auto new_remains = remains;
        new_remains.erase(n);
        PrintPermutation(new_prefix, new_remains);
    }
}

// 有一种最好的实现方法，就是允许迭代
// 我们只需要遍历这个东西 就可以直接不断的获取下一个排列
// 这需要我们实现一个迭代器
// 因为在C++20以下我们无法使用协程来完成，只能自己手动实现一个迭代器
// 咱们尝试一下
void GeneratePermutation(uint64_t n) {

    // 我们首先有一个序列，代表前缀 suffix
    // 还有一个集合，set，用来递归做接下来的全排列

    for (uint64_t i = 1; i <= n; i++) {
        std::vector<uint64_t> range = GenerateRange(1, n + 1);
        auto set = std::set<uint64_t>{range.begin(), range.end()};
        set.erase(i);

        PrintPermutation(std::vector<uint64_t>{i}, set);
    }

    // 我发现算法中经常需要生成一个range
    // 而C++17中并没有这个range
    // 所以最好是实现一个
}

void PrintPermutationV2(int n, int* A, int cur) {
    if (cur == n) {
        // for (int i = 0; i < n; i++) {
        //     std::cout << A[i] << ", ";
        // }
        // std::cout << "\n";
    } else {
        for (int i = 1; i <= n; i++) {
            bool ok = true;
            // 我们都不用初始化数组，因为这里的判断是j < cur
            // 一开始cur是零，甚至都不会走这个循环
            for (int j = 0; j < cur; j++) {
                if (A[j] == i) {
                    ok = false;
                }
            }

            if (ok) {
                A[cur] = i;
                PrintPermutationV2(n, A, cur + 1);
            }
        }
    }
}

// struct Permutation {

//     struct PermutationIterator {

//         PermutationIterator operator++() {

//             // 在这里想模拟coroutine的操作
//             // 每次调用迭代器，都会执行一段计算
//             // 但是因为这是一个递归的函数
//             // 我们不太容易达到这一点
//             // 所以协程真的很方便
//             // C++23中的generator完美的做到了这一点

//         }
//     };

//     auto begin() {}

//     auto end() {}
// };

} // namespace

int main() {

    int n = 10;

    // 12455 ms
    auto start = std::chrono::system_clock::now();
    GeneratePermutation(n);
    auto end = std::chrono::system_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();
    std::cout << duration << " ms" << std::endl;

    // for (auto permutation : GeneratePermutation(3)) {
    //     Print(permutation);
    // }

    // 我们的方法完败，书上的方法完胜！
    // 0 ms
    start = std::chrono::system_clock::now();
    int array[256];
    PrintPermutationV2(4, array, 0);
    end = std::chrono::system_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();
    std::cout << duration << " ms" << std::endl;
}
