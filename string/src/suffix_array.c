// 2022/12/8
// zhangzhong
// suffix array

#include "string/suffix_array.h"
#include "util/c_random.h"
#include "util/c_string.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// 在这里重新实现排序

// 这个交换有点慢了
// 但是如果保存指针的话 编程就太复杂了
static void swap(suffix_t* suffix, int i, int j) {
    suffix_t tmp = suffix[i];
    suffix[i] = suffix[j];
    suffix[j] = tmp;
}

static void insertion_sort(suffix_t* suffix, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        for (int j = i; j > low; j--) {
            if (strcmp(suffix[j - 1].str, suffix[j].str) > 0) {
                swap(suffix, j - 1, j);
            } else {
                break;
            }
        }
    }
}

static int random_middle(suffix_t* suffix, int low, int high) {
    // 随机取三个数
    // 这里的随机会导致问题
    // 用新的随机数的实现
    // rand_t r = make_rand(low, high);
    // int i = crand(&r);
    // int j = crand(&r);
    // int k = crand(&r);
    int i = rrand(low, high);
    int j = rrand(low, high);
    int k = rrand(low, high);

    // 取三个的中位数
    // 类似插入排序的方式比较他们
    if (strcmp(suffix[j].str, suffix[i].str) < 0) {
        // i < j
        // 那我们还要看i和k的关系
        // 交换i和j
        int tmp = i;
        i = j;
        j = tmp;
    }

    // 现在i j就是有序的
    // 然后比较
    if (strcmp(suffix[k].str, suffix[j].str) < 0) {
        // k < j
        // 交换
        int tmp = k;
        k = j;
        j = tmp;

        if (strcmp(suffix[j].str, suffix[i].str) < 0) {
            int tmp = i;
            i = j;
            j = tmp;
        }
    }

    assert(strcmp(suffix[i].str, suffix[j].str) <= 0);
    assert(strcmp(suffix[j].str, suffix[k].str) <= 0);
    return j;
}

static int partition(suffix_t* suffix, int low, int high) {
    // 随机三数取中
    // 交换mid和low
    int mid = random_middle(suffix, low, high);
    swap(suffix, low, mid);

    // 然后执行正常的划分
    suffix_t* pivot = &suffix[low];
    int i = low + 1;
    int j = high;
    while (true) {
        while (i < high && strcmp(suffix[i].str, pivot->str) <= 0)
            i++;

        while (j > low && strcmp(suffix[j].str, pivot->str) >= 0)
            j--;

        if (i < j) {
            swap(suffix, i, j);
        } else {
            break;
        }
    }

    swap(suffix, low, j);
    return j;
}

static void quick_sort_impl(suffix_t* suffix, int low, int high) {
    // 实现两个额外的优化
    // 1. 小数组进行插入排序
    // 2. 随机取三数取中位数
    if (high <= low) {
        return;
    }
    if (high - low < 16) {
        insertion_sort(suffix, low, high);
        return;
    }

    int p = partition(suffix, low, high);
    quick_sort_impl(suffix, low, p - 1);
    quick_sort_impl(suffix, p + 1, high);
}

static void msd_sort(suffix_t* suffix, int n) {
    quick_sort_impl(suffix, 0, n - 1);
}

suffix_array_t* make_suffix_array(char* str, size_t n) {
    suffix_array_t* sa = (suffix_array_t*)malloc(sizeof(suffix_array_t));
    sa->n = n;
    sa->suffix = (suffix_t*)malloc(sizeof(suffix_t) * sa->n);
    // 把str的每一个后缀插入suffix数组中
    int line = 1;
    int col = 1;
    for (size_t i = 0; i < sa->n; i++) {
        // sa->suffix[i] = str + i;
        // 现在不仅要记录后缀 还要记录它在文件中的位置
        sa->suffix[i].str = str + i;
        sa->suffix[i].line = line;
        sa->suffix[i].col = col;

        if (str[i] == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
    }
    // 然后排序
    // 这个排序也得重新写了 得根据我们的要求来
    msd_sort(sa->suffix, sa->n);
    return sa;
}

void free_suffix_array(suffix_array_t* sa) {
    if (sa) {
        free(sa->suffix);
        free(sa);
    }
}

int lcp(char* str1, char* str2) {
    int i = 0;
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
        i++;
    }
    return i;
}

string_t lrs_slow(char* str) {
    // 双层循环 对于所有位置的字符串之间 执行lcp
    // 记录最长的重复序列
    int len = strlen(str);

    int max_len = 0;
    int max_pos = 0;
    int n = 0;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            n = lcp(&str[i], &str[j]);
            if (n > max_len) {
                max_len = n;
                max_pos = i;
            }
        }
    }

    string_t repeated_str = make_string("");
    string_concat(&repeated_str, str + max_pos, max_len);
    return repeated_str;
}

// 这个玩意要怎么返回呢
string_t lrs(char* str) {
    suffix_array_t* sa = make_suffix_array(str, strlen(str));

    // 然后依次检查相邻的最长公共子前缀
    // 并记录最大的两个
    // 最好能返回位置呀
    // 我发现我们的后缀数组实现在排完序之后 没有之前的位置信息了
    // 怎么没有了呢 数组的元素就是字符串指针呀 这不就是位置信息了
    // 这就是信息本息啊

    int max_len = 0;
    size_t max_pos = 0;
    int len = 0;
    for (size_t i = 1; i < sa->n; i++) {
        len = lcp(sa->suffix[i - 1].str, sa->suffix[i].str);
        if (len > max_len) {
            max_len = len;
            max_pos = i - 1;
        }
    }

    // 最后返回一个啥呢??
    // 最起码返回两个位置吧  也就两个最长的公共子序列的位置
    // 不是的 是返回这个字符串本身
    // 就是重复的序列
    // 讲道理 我们可以顺道返回这个重复的序列出现的所有位置呀
    string_t repeated_str = make_string("");
    string_concat(&repeated_str, sa->suffix[max_pos].str, max_len);
    free_suffix_array(sa);
    return repeated_str;
}

// 为什么这样返回的是小于等于sa的key的数量？？
int suffix_array_search(suffix_array_t* sa, char* str) {
    int low = 0;
    int high = sa->n - 1;
    int mid = 0;

    while (low <= high) {
        mid = low + (high - low) / 2;
        int r = strcmp(str, sa->suffix[mid].str);
        if (r == 0) {
            return mid;
        } else if (r < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    // 为什么return low
    // 就是return low
    // 后缀数组有一个非常有用的特点 他是不重复的！！！
    // 所以二叉查找一定是正确的
    // 他会返回>=key的第一个元素
    return low;
}

// suffix比较函数
// int suffix_cmp(suffix_t *lhs, suffix_t *rhs) {
//   if (lhs->line < rhs->line) {
//     return -1;
//   } else if (lhs->line > rhs->line) {
//     return 1;
//   } else {
//     // lhs->line == rhs->line
//     if (lhs->col < rhs->col) {
//       return -1;
//     } else if (lhs->col > rhs->col) {
//       return 1;
//     }
//     return 0;
//   }
// }

// void insertion_sort_by_line_and_col(suffix_t *suffix, size_t size) {
//   // i从1开始
//   for (size_t i = 1; i < size; ++i) {
//     for (size_t j = i; j > 0; --j) {
//       if (suffix_cmp(&suffix[j - 1], &suffix[j]) > 0) {
//         swap(suffix, j - 1, j);
//       } else {
//         // 说明两者满足less qeual的关系
//         // 那么这轮比较就可以结束了
//         break;
//       }
//     }
//   }
// }

// pattern是不是str的前缀
static bool is_prefix(char* str, char* pattern) {
    while (*str && *pattern && *str == *pattern) {
        str++;
        pattern++;
    }
    return *pattern == '\0';
}

// #define ANSI_COLOR_RED "\x1b[31m"
// #define ANSI_COLOR_GREEN "\x1b[32m"
// #define ANSI_COLOR_YELLOW "\x1b[33m"
// #define ANSI_COLOR_BLUE "\x1b[34m"
// #define ANSI_COLOR_MAGENTA "\x1b[35m"
// #define ANSI_COLOR_CYAN "\x1b[36m"
// #define ANSI_COLOR_RESET "\x1b[0m"

// todo 想要实现一个这样的kwic并不算特别复杂 但是最好还是用C++实现
// 假设我们已经建立了后缀数组
// 现在用于输入了一个str
// 希望我们可以在被建立索引的文本中找到str
void kwic(suffix_array_t* sa, char* str) {
    int i = suffix_array_search(sa, str);
    int begin = i;
    // 然后我们从此节点开始，一直向后寻找
    // 一直找到哪儿呢
    // 一直找到前缀不是的地方
    for (; i < sa->n && is_prefix(sa->suffix[i].str, str); i++) {
        // 咱们可以输出该行
        // 但是行号从哪里来呢
        // 可以在建立映射的时候来
        // 我们在读取文件建立后缀数组的时候 同时把文件的行号也带上

        // 当然现在为了简单 我们也可以直接输出此字符串
        // 输出对应的行和起始位置以及终止位置
        printf("%d: %d{%s}%d\n", sa->suffix[i].line, sa->suffix[i].col, str,
               sa->suffix[i].col + (int)strlen(str));
        // 还有一个更好的输出方法，就是按照行号排序，相同行的情况只输出一行
        // 并且匹配的字符的输出颜色应该加深
        // 但是这个就不太会了
        // 怎么控制输出字符的颜色呢
        // 好了 问题解决了 那么我们现在就实现这个功能

        // printf(ANSI_COLOR_RED "This text is RED!" ANSI_COLOR_RESET "\n");
        // printf(ANSI_COLOR_GREEN "This text is GREEN!" ANSI_COLOR_RESET "\n");
        // printf(ANSI_COLOR_YELLOW "This text is YELLOW!" ANSI_COLOR_RESET
        // "\n"); printf(ANSI_COLOR_BLUE "This text is BLUE!" ANSI_COLOR_RESET
        // "\n"); printf(ANSI_COLOR_MAGENTA "This text is MAGENTA!"
        // ANSI_COLOR_RESET "\n"); printf(ANSI_COLOR_CYAN "This text is CYAN!"
        // ANSI_COLOR_RESET "\n");

        // 首先是按照行号 col 号进行排序
        // 因为搜索结果往往较少 所以使用插入排序即可
    }

    // int end = i;
    // // 把需要处理的suffix复制出来
    // int len = end - begin + 1;
    // suffix_t *suffix = (suffix_t *)malloc(sizeof(suffix_t) * len);
    // for (int i = begin; i < end; i++) {
    //   suffix[i - begin] = sa->suffix[i];
    // }

    // // 排序
    // insertion_sort_by_line_and_col(suffix, len);
    // // 然后输出
    // int line = 0;
    // int current_line = 0;
    // i = 0;
    // int col;
    // while (true) {
    //   // 一次输出一行
    //   line = suffix[i].line;
    //   // 首先输出行号
    //   printf("%d", line);
    //   printf(":");
    //   // 持续读取直到下一行的行号变了
    //   // 然后我们可以输出到suffix[i].col的位置
    //   // 不对啊 我们根本不知道这一行的开始在哪里
    //   // 哎 不对 我们直到 col会告诉我们
    //   // 为什么没有一个函数可以控制输出的字符数量??
    //   suffix->str - suffix[i].col i++;
    //   // 不行 这个编程也太困难了
    //   // 还是算了

    //   while (true) {
    //     if (i + 1 >= len) {
    //       break;
    //     }

    //     if (suffix[i + 1].line == line) {
    //       // 仍然是当前行
    //       // 我们只能从当前位置继续输出至 col 位置
    //     } else {
    //       // 下一行了
    //       break;
    //     }

    //     i++;
    //   }
    //   i++;
    // }
}
