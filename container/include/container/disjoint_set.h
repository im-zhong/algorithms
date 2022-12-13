// 2021/9/17
// zhangzhong
// 不相交集
// disjolong set
// 等价问题，什么是等价问题??
//
// (a, b), a, b 属于 S,
// a R b is true or false
// 在集合S上定义关系 reletion R
// 如果 a R b = true, 我们说a与b有关系
//
// 等价关系 equivalence relation
// 自反 所有的a属于S，a R a = true
// 对称 a R b == b R a
// 传递 a R b, b R c => a R c
//
// 一个元素a属于S的等价类equivalence class是S的一个子集
// 它包含所有与a有关系的元素

// 输入：N个集合collection, 每个集合仅有一个元素。
// 除了自反，所有的关系均为false
// 每个集合均有一个不同的元素，从而所有的集合不相交 disjoint
//
// 定义两个运算:
// 1. find, 返回包含给定元素的集合
// 2. add_releation, 添加关系 a R b
//   可以通过find(a) find(b) 查看他们时候在同一个子类中
//   如果不在，使用求并运算 union, 把a和b两个子类合并成一个新的等价类
// 最终的结果是合并了两个不相交集，生成一个新的不相交集
// 这是在disjolong set 上面的 find/union 操作
//
// 不进行任何比较操作，而只需要知道他们的位置
// 假设所有元素已经从1 - N 进行排列
//
// 集合的名字是相当随意的
// 只需要让 find(a) == find(b)即可
//

#ifndef __DISJOINT_SET_H__
#define __DISJOINT_SET_H__

#include <stddef.h>
#include <stdlib.h>
// 这个的具体处理方法可以参考heap的实现
typedef struct {
    // 树的集合，也就是一个森林
    // 这里的下标最好还是用long
    long* set_forest;
    size_t size;
} disjoint_set;

// 还是按照 秩 rank 来union，感觉会好一点
// 通常来说，我是不需要获得某个类的大小的
// 如果你需要获得更多的信息，建议直接使用红黑树来表示集合

// 不对啊，这里面0这个位置是可以使用的
// 因为我表示的是子类的大小，
// 0 1 2 3 4 5 6 7 8 9
//   0 0 0 0 0 0 0 0 0
// union(5, 6)
//   0 0 0 0 0 5 0 0 0

static inline void disjoint_set_init(disjoint_set* djs, size_t size) {
    // 将所有的元素初始化为-1 表示他是这个集合的树的根
    // 根的数值一定小于零 这样才能表示根
    // 根的数值的绝对值表示这个集合的元素数量
    // 根的下标表示集合的名字
    // 孩子节点的值是其父节点的下标
    djs->set_forest = (long*)malloc(size * sizeof(long));
    djs->size = size;
    for (size_t i = 0; i < size; ++i) {
        djs->set_forest[i] = -1;
    }
}

static inline void disjoint_set_free(disjoint_set* djs) {
    free(djs->set_forest);
    djs->set_forest = NULL;
    djs->size = 0;
}

// path compression
// 路径压缩, 尽可能减少树的深度
// find(x)， 从x到根路径上的每一个节点，都使他的父节点变成根
// 这个find必须用递归实现
// O(logN)
static inline long disjoint_set_find(disjoint_set* djs, long set) {
    // 说明这个集合的根 直接返回
    if (djs->set_forest[set] < 0) {
        return set;
    } // 因为最终递归结果会返回结合的根 所以这里就可以路径压缩
    // 让子节点直接指向根
    return djs->set_forest[set] = disjoint_set_find(djs, djs->set_forest[set]);
}

// union by size
// 数组的根保存的不是零，而是一个负数，表示的是子类的大小
// O(log N)
static inline void disjoint_set_union(disjoint_set* djs, long lhs, long rhs) {
    lhs = disjoint_set_find(djs, lhs);
    rhs = disjoint_set_find(djs, rhs);

    if (lhs == rhs) {
        return;
    }

    // 就是把小的集合合并到大的集合里面
    // 修改小的集合的根指向大的结合的根
    // 大的集合的元素数量加起来 就可以了
    // lhs = -1
    // rhs = -2
    if (djs->set_forest[lhs] < djs->set_forest[rhs]) {
        djs->set_forest[lhs] += djs->set_forest[rhs];
        djs->set_forest[rhs] = lhs;
    } else {
        // -1 >= -2
        //
        djs->set_forest[rhs] += djs->set_forest[lhs];
        djs->set_forest[lhs] = rhs;
    }
}

// int main(int argc, char* argv[])
// {
//     disjoint_set djs;
//     disjoint_set_init(&djs, 1024);

//     disjoint_set_find(&djs, 100);
//     disjoint_set_union(&djs, 4, 60);

// }

#endif // __DISJOINT_SET_H__