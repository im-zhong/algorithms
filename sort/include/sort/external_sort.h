// 2023/3/13
// zhangzhong
// external sort

#ifndef EXTERNAL_SORT
#define EXTERNAL_SORT

#include <stdbool.h>
#include <stddef.h>

// 这个接口非常的简单
// input 里面存放在输入数据
// 我们假设输入数据是int 4B大小一个
// 然后我们的memory限制在 4K 也就是一页
// 而我们将要排序4M 个数据 也就是 1000倍于我们内存的数据
// 不对 这是测试需要考虑的事情 算法实现上不应该依赖于这些常数
// 总的来说就是给定一个输入文件 然后将排好序的数据输出到输出文件即可
void external_sort(const char* input, const char* output);

// 同样我们需要一个函数来测试最终的文件数据是否是有序的
// 这个函数可以取巧  只要最终所需要排序的数据不超过我们内存的大小
// 我们就可以把整个文件读入内存进行排序 简化实现
bool check_external_order(const char* random_file, const char* ordered_file,
                          size_t num);

#endif // EXTERNAL_SORT
