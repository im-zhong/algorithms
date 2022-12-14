// 2022/11/20
// zhangzhong
// def

#ifndef _DEF_H_
#define _DEF_H_

#include <stdint.h>

// 对整个项目的基础宏 函数等等做一些说明
// 每个文件都应该包含此文件

// 真的没办法了 只能用value_t了 我真是吐了
// 为什么标准库会占用key_t呢？？？
typedef int64_t value_t;

// 我们还可以定义一些debug函数
#define debug

#endif // _DEF_H_