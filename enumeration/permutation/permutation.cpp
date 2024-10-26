// 2024/10/25
// zhangzhong

// generate 1~n permutaion
#include <algorithm>
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

// 尝试枚举给定的数组，数组P保证是有序的, 无重复的
void PrintPermutationV3(int n, char* P, char* A, int cur) {
    // 递归结束条件
    if (cur == n) {
        // print the A
        for (int i = 0; i < n; i++) {
            std::cout << A[i] << ", ";
        }
        std::cout << "\n";
    } else {
        // 枚举P中的所有元素
        for (int i = 0; i < n; i++) {

            bool ok = true;
            // 检查当前元素P[i] 是否出现在A[0-cur]中
            for (int j = 0; j < cur; j++) {
                if (A[j] == P[i]) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                // 这里忘了设置A了
                A[cur] = P[i];
                PrintPermutationV3(n, P, A, cur + 1);
            }
        }
    }
}

void PrintPermutationV4(int n, char* P, char* A, int cur) {

    if (cur == n) {
        // print the A
        for (int i = 0; i < n; i++) {
            std::cout << A[i] << ", ";
        }
        std::cout << "\n";
    } else {
        for (int i = 0; i < n; i++) {
            // 重复元素只枚举一次
            if (i != 0 && P[i] == P[i - 1]) {
                continue;
            }

            // 这里就是之前检查已经被使用的元素的地方
            // 因为可能存在重复的元素
            // c1 = 所以要检查在A[0:cur]中P[i]出现的次数
            // c2 = 和P[i]在P中的个数
            // 只要 c1 < c2， 我们就是okde
            bool ok = true;
            int c1 = 0;
            for (int j = 0; j < cur; j++) {
                if (A[j] == P[i]) {
                    c1 += 1;
                }
            }

            int c2 = 0;
            for (int j = 0; j < n; j++) {
                if (P[j] == P[i]) {
                    c2 += 1;
                }
            }

            ok = c1 < c2;

            if (ok) {
                A[cur] = P[i];
                PrintPermutationV4(n, P, A, cur + 1);
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

    int n = 4;

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

    char str[4] = "ABC";
    char permutation[4] = {};

    PrintPermutationV3(3, str, permutation, 0);

    // if we give the repeated sequence, then V3 will be wrong
    char str1[4] = "AAA";
    PrintPermutationV3(3, str1, permutation, 0);
    // 可以看到没有任何输出，因为算法假设P数组中没有任何重复
    // 所以在枚举时，就会便利A[0-cur]，并设置A[cur]为不重复的元素
    // 但是P数组中全都是重复的，所以函数无法递归到结束条件达成，所以没有任何输出

    std::cout << "V4 for AAA\n";
    PrintPermutationV4(3, str1, permutation, 0);

    std::cout << "V4 for AAB\n";
    char str2[4] = "AAB";
    PrintPermutationV4(3, str2, permutation, 0);

    // cpp algorithm next permutation
    // TODO: C++STL中的所有算法，所有容器都得研究明白！
    // 自己全部实现一遍！但是不考虑范型，只考虑其算法原理
    // https://en.cppreference.com/w/cpp/algorithm/next_permutation

    std::string s = "aba";

    do {
        std::cout << s << '\n';
    } while (std::next_permutation(s.begin(), s.end()));

    std::cout << s << '\n';
}
