// 2022/11/5
// zhangzhong
// B Tree
#ifndef _BTREE_H_
#define _BTREE_H_

#include "array.h"
#include <stdbool.h>
#include <stddef.h>

// 每个节点含有k个关键字 就有k+1个孩子
// 关键字从小到大排列 那么就可以按序查找
// 孩子节点的数量有限制 最小是 minimum 最大是minimum*2
#define MINIMUM_DEGREE 2

typedef struct btree_node {
    // 当前节点是否为叶子节点
    bool is_leaf;
    // 当前节点存储的关键字个数
    size_t size;
    // key 我们使用数字作为关键字 毕竟不是要用的 只是用来学习的
    value_t keys[MINIMUM_DEGREE * 2 - 1];
    struct btree_node* childs[MINIMUM_DEGREE * 2];
} btree_node_t;

// 用户直接接触的数据结构 代表b树
typedef struct btree {
    struct btree_node* root;
} btree_t;

bool btree_search(btree_t* root, value_t key);

// 创建一个空的b树
void btree_init(btree_t* root);

void btree_insert(btree_t* btree, value_t key);

void btree_free(btree_t* root);

void btree_delete(btree_t* btree, value_t key);

bool btree_is_empty(btree_t* btree);

// test
// bool btree_check(btree_t *btree);

// 2021/9/22
// zhangzhong
// btree

// 阶为M的B树
// 树的根或者是一片树叶，或者其儿子数在2-M之间
// 除根外，所有非树叶的儿子数在 [M/2(向上取整)] - M 之间
// 所有的树叶都在相同的深度上
// 所有的数据都存储在树叶上
//

// 每一个内部节点上皆含有指向该节点各儿子的指针
// 和分别代表在子树中发现的最小关键字值
// 对于每一个节点，子树T1的所有关键字都小于子树T2，以此类推
// 树叶包含所有实际数据,
// 这些数据可以是关键字本身，或者指向关键字结构的，blob
//

// B树主要考虑磁盘存取次数
// 一次从磁盘中读取一页或者多页数据， page 2k 4k ...
//

// 主要考虑的因素：
// 磁盘存取次数
// cpu计算时间

// 使用读出或写入磁盘的页数来衡量磁盘存取次数
// B树算法将所需页面复制到主存，然后将修改过的页面写回磁盘
// 一个B树算法的运行时间主要由它执行的disk IO操作的次数相关
// 所以每次操作都应该读写尽可能多的信息
// 因此，一个B树节点通常和一个完整磁盘页一样大
// 并且一个磁盘页的大小限制了一个B树结点可以含有的孩子的个数
// 也就是一页的大小可以放下多少个关键字
// 4k / 4 = 1000

// 算法导论书中图18-3中，如果我们把第二层页存在放主存中，只不过使用了1k
// page的内存
// 这是相当少的内存，但是磁盘读取次数就仅剩余一次了，大大加快了速度
//

// 如果程序崩溃的话，更新的数据就丢失了，所以还要定期flush磁盘
//

// linux内核实现的b+tree
// 但是仅仅只是一个内存中的树，这种树没啥意义
// 还是算法导论中描述的磁盘IO的b+tree更有意义
// 但是这个实现起来可能就有一些困难了
// 但是其实怎么找都是可以写的
// 而且分支数量可以是动态的, 只不过btree一旦定义了，这个分支数量就不能改了
//

// B树的定义十分复杂，见 算法导论 18.1
// 每个节点内部的值都是有序排放的，这样可以有二叉搜索的优化可能
// 每个节点包含的关键字个数有上界和下界
// b树的最小度数 minimum degree = t, 每个节点至少有t个孩子，至多有2t个孩子
// 如果一个节点恰好有2t-1个关键字，这个节点是full满的
// t越大，树越矮

// #include <stdbool.h>
// #include <stddef.h>
// #define btree_minimum_degree 2

// typedef struct btree_node
// {
//     // 用户的关键字类型我们也是不知道的
//     // 这个需要用户自己是实现了
//     // 那么这里是不需要提供的
//     // 仅仅实现btree的最核心的操作

//     // 每个节点都要保存 n 个关键字 和 n + 1 个指向孩子节点的指针
//     // 这些应该是保存在数组里面的

//     // 最多有 2t - 1 个关键字
//     value_t keys[2 * btree_minimum_degree - 1];
//     struct btree_node *childs[2 * btree_minimum_degree];

//     // 虽然capacity是 2t, 但是实际的元素数目还是需要额外保存的
//     value_t size;

// }btree_node;

// // 叶子节点上面应该存储的是不同的东西

// // 写这些数据结构面临着和红黑树一样的麻烦
// // 类似find insert这样的操作都必须由用户二次编程来完成
// // 无法提供一个通用的接口，因为不知道用户类型是什么
// // 除非，我们认为key是一个void*

// // b tree search
// void btree_find(btree_node *head, value_t key)
// {
//     // 相比二叉树，在每一层进行双路选择
//     // 这里其实就是进行一个多路选择而已

//     while (head)
//     {
//         // 在每一层中找到对应的下标
//         value_t i = 0;
//         for (; i < head->size && key < head->keys[i]; ++i);

//         // 通过这个下标，我们可以找到对应的child
//         head = head->childs[i];
//         // 还有一个问题，我们在获取每一个node的时候，我们可能需要读取磁盘
//         // 也可能从缓存中获得，这个可以封装成一个函数，从而不影响整体逻辑
//     }
//     // 不断循环，直到找到
// }

// // insert
// // 在b树中，不能简单的创建一个新的叶节点,
// 然后将其插入，因为这样得到的树不是合法的B树
// // 我们需要将新的关键字插入一个已经存在的叶子上
// // 如果不是满的，我们可以直接写入；如果是满的，我们引入一个分裂split操作
// // 将一个满的节点(2t-1)个关键字，按照中间关键字, 分裂成两个各含 t-1
// 个关键字的节点
// // 中间关键字被提升到该节点的父节点。
// // 如果父节点也是满的，就继续向上传播

// //
// 自顶向下的插入？？为什么b树这么搞，而红黑树就不这么搞呢？？是因为磁盘IO吗

// // 检查某个节点是否是满的
// bool btree_is_full(btree_node *head)
// {
//     return head->size == 2 * btree_minimum_degree - 1;
// }

// void btree_split(btree_node *head, btree_node *node, size_t split_index)
// {
//     // 假设非满的内部节点x,
//     // 有一个下标 i, x->child[i] 是一个满的子节点
//     // 把子节点分裂成两个，并调整x使之包含两个新的孩子

//     // 如果根满了，首先需要创建一个新的根，是旧根成为新根的孩子
//     // 然后才能执行分裂。树的高度加1
//     // 分裂是树长高的唯一途径

// }

// // 删除一个关键字
// // 与插入正好相反，必须保证在删除之后，节点不会太小
// // 但是根节点除外

// // b树的思想是相当简单明了的
// // 比较困难的是，整体的架构需要如何设计
// // 因为B树往往使用在数据库系统中，为了加快读写速度，应该尽可能减少磁盘IO
// //
// 并增加每次IO的量，需要应该额外的缓存技术，这个对于一个简单的cstl项目而言
// // 规模颇大，不适合作为练习实现
// // 所以B树看完算法导论的介绍之后，择机实现吧

#endif // _BTREE_H_
