// 2021/9/23
// zhangzhong
// spares graph, 稀疏图，边的条数|E|远远小于|V|^2

#ifndef __SPARE_GRAPH_H__
#define __SPARE_GRAPH_H__

// 使用邻接链表
// G = (V, E)
//
//   1 2 3 4 5
// 1   1     1
// 2 1   1 1 1
// 3   1   1
// 4   1 1   1
// 5 1 1   1
//
// 1 -> 2 -> 5
// 2 -> 1 -> 5 -> 3 -> 4
// 3 -> 2 -> 4
// 4 -> 2 -> 5 -> 3
// 5 -> 4 -> 1 -> 2
//
// 上面第一个是邻接矩阵，12345表示的是某一个节点
// 矩阵的值1表示有一条边存在。
// 第二个表示是邻接链表，每个节点都有一条链表，
// 这个链表含有的值就是与1邻接的所有节点
// 这两种方式都可以表示无向图和有向图
//
// 因为图是一个非常抽象的东西，所以我们可以直接对图的节点进行编号
// 就像在disjoint_set里面那样，直接用long表示节点即可
// 因为这些数据结构往往不作为容器来使用，所以不需要考虑存放各种类型的数据
// 它只是描述一个问题，抽象！
//

// 权重图，weight
// 每条边带有一个相关的权重 weight
// E(u,v)的weight可以保存在u->v的链表节点上

// 邻接链表的存储空间需求为O(E + V)
// 邻接矩阵的存储空间需求为O(E^2)

// 那么在这个文件里面，我将实现一个邻接矩阵表示的稀疏图

#include "container/disjoint_set.h"
#include "container/heap.h"
#include "container/index_heap.h"
#include "container/queue.h"
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// 但是，我们的entry是什么呢？？

typedef struct __spare_graph {
    // 稀疏图实际上就是一个链表数组

    // 每一个节点应该有一个独一无二的标识，
    // 这个标识实际上也应该由应用程序来决定，而不是库来决定
    // 但是这里使用序号代替 也就是long或者int
    // 这个可以使用一个typedef来定义, 就叫...叫啥好
    // 因为这样可以直接根数组下标进行一个交互

    // 图的所有节点
    // 链表数组
    // 如果是用链表数组来表示的话 那么我们是不能随意的增加节点的
    // 因为节点的数量在一开始就规定好了 我们能做的就是增加和减少边
    // 作为一个学习例子来讲是足够的 而且相对来说实现也比较简单 就酱吧
    list_node_t* adjacency;
    size_t size;
} spare_graph_t;

typedef int64_t vertex_t;
// edge
typedef double weight_t;

// 广度优先搜索
// G = (V, E) 和一个源点 s
// 按照广度进行搜索，也就是说，搜索完所有与s直接邻接的节点之后，才会搜索其他节点
//

typedef struct {
    vertex_t vertex;
    weight_t weight;

    // 如果这里不需要写这个 继承队列 就好了
    // 就更加灵活
    // 看来还是没有必要多此一举啊，越灵活越好
    // INHERIT_QUEUE;
    list_node_t node;
} graph_entry_t;

static inline graph_entry_t* make_graph_entry(vertex_t vertex, weight_t weight);

typedef struct {
    vertex_t v1;
    vertex_t v2;
    weight_t weight;
    list_node_t node;
} edge_t;

edge_t* make_edge(vertex_t from, vertex_t to, weight_t weight);

// 首先调用这个函数初始化一个图
// 然后调用 insert_edge 和 delete_edge 来改变图
// 但是图的节点数量一经初始化就不能在改变了
void spare_graph_init(spare_graph_t* graph, size_t size);

void spare_graph_free(spare_graph_t* graph);

// 对于一个图来说 我们可以加入很多东西啊
size_t spare_graph_size(spare_graph_t* graph);

size_t spare_graph_edge(spare_graph_t* graph);

typedef void (*handle)(vertex_t from, vertex_t to, weight_t weight);

// 这实际上是遍历所有的边
void spare_graph_for_each_edge(spare_graph_t* graph, handle handle);

// 我就想知道某条边的weight
weight_t spare_graph_weight(spare_graph_t* graph, vertex_t from, vertex_t to);

// 我想加一条边  我想减一条边 这都是可以的吧
void spare_graph_insert_edge(spare_graph_t* graph, vertex_t from, vertex_t to,
                             weight_t weight);

void spare_graph_delete_edge(spare_graph_t* graph, vertex_t from, vertex_t to);

// 咱们不要这个color了 实际实现是不需要的
// enum vertex_color { white, black, gray };

typedef struct {
    // 这里的color可以变成一个bool，其实就是有没有访问的区别
    bool visited;
    // prev用于生成广度优先搜索树
    vertex_t prev;
    // distance用于生成最短路径
    size_t distance;
} bfs_t;

// vertex queue
// 没有模板就是这么恶心
typedef struct {
    vertex_t vertex;
    INHERIT_QUEUE;
} vqueue_entry_t;

vqueue_entry_t* make_vqueue_entry(vertex_t vertex);

// breadth-first search
// bfs
// bfs会生成一个bfs 结果
// 包含最短路径和最小生成树
// 内部malloc 调用者应该free
bfs_t* bfs(spare_graph_t* graph, vertex_t vertex);

// 广度优先搜索，可以给出从源节点s到所有可以到达的节点之间的距离
// 这个结果会记录在一个数组里面
// 广度优先树
// 如果生成了广度优先数，我们可以给出从s到v的最短路径
// 这非常简单，从v开始不断向前查找前驱即可，直到找到s
// 这个过程可以借助一个递归简单的完成
// shortest_path
// 我们的这个深度优先搜索，应该在遍历的时候，做一些事情吧
// 其实入队出队的节点顺序就是深度优先顺序，可以有两种方式
// 用户可以提供一个函数，在深度优先搜索的时候干
// 我们会针对每一个entry调用该函数
// 然后，我们可以返回一个广度优先树，其实就是那个vertex_property数组
// 不如就把他的名字改成 广度优先树 bftree， 就酱~~~
// 根据bfs_t 你可以很快直到源点到所有其他节点的最短路径 已经最短路径上的节点
void bfs_path_impl(spare_graph_t* graph, vertex_t from, vertex_t to,
                   bfs_t* bfs);

void bfs_path(spare_graph_t* graph, vertex_t from, bfs_t* bfs);

// -------------------------------------------------------

// 深度优先搜索
// 只要可能，就在图中尽量深入
// 递归，很容易实现，特别类似二叉树的根序遍历

// ?? 深度优先搜索有多个源节点？？？ how to
// 深度优先同样可以记录前驱节点
// 不过，广度优先是一颗 广度优先树
// 而，深度优先前驱子图，是一颗由多颗深度优先树构成的深度优先森林
// 可以使用颜色来防止重复访问
// 所有的深度优先树是不相交的

// 时间戳？？
enum graph_color { white, gray, black };

typedef struct {
    // 不对啊，我为什么要在这里面记录vertex
    // 数组的下标就是vertex啊
    // vertex_t vertex;
    // 前驱
    // 与广度优先搜索不同 dfs的前驱形成森林
    vertex_t prev;
    // 这里是两个时间戳,一个是入队的时间戳
    // 我傻逼了，dfs哪有队列。。。
    // 第一个时间就是visit时间，也就是第一次访问到这个节点的时间
    // 也就是涂成灰色的时间
    size_t visit_time;
    // 因为是递归实现的，所以他需要回溯，第二个时间就会回溯回来的时间
    // 每次访问一个新的节点都会使得time++
    // 也就是变成黑色的时间
    // 完成所有邻接节点访问的时间
    size_t finish_time;
    // 节点颜色
    int color;
} dfsforest_t;

void dfs_visit(spare_graph_t* graph, dfsforest_t* dfsforest, vertex_t vertex,
               size_t* time);

// depth-first search
// O(E + V)
dfsforest_t* dfs(spare_graph_t* graph);

typedef struct {
    vertex_t vertex;
    list_node_t node;
} vlist_entry_t;

vlist_entry_t* make_vlist_entry(vertex_t vertex);

// 这个深度优先实现起来要简单的多
// 因为不涉及队列，仅仅是一个简单的递归

// 深度优先搜索提供了树的结构价值很高的信息
// 算法导论 图22-4, 一个典型的深度优先搜索过程, 注意时间戳
// 每个节点的发现时间与完成时间

// 其实就是dfs的变种
// 就是在节点被涂成黑色的时候插入链表头部
void toposort_impl(spare_graph_t* graph, vertex_t vertex, int* color,
                   list_node_t* topo_list);

// 拓扑排序
void toposort(spare_graph_t* graph, list_node_t* topo_list);

bool spare_graph_has_circle_impl(spare_graph_t* graph, vertex_t vertex,
                                 int* color);

// 环路判断非常简单 同样也是用dfs实现
// 只要碰到一条back path 就直接报告有环路
// 一个稍微复杂一点的问题是 报告环路路径
// 我们需要记录每个节点的dfs搜索前驱 这样碰到环路是 就可以向上搜索 从而给出环路
bool spare_graph_has_circle(spare_graph_t* graph);

// 用于mst spt算法
bool weight_less(const void* lhs, const void* rhs);

// todo: 我想要一个无向图判断环路的方法
// 非常简单 和kruskal的思想一样
// 遍历所有的边 每次碰到两个不在同一集合的边 就让两个集合合并
// 一旦碰到一条新的边 连接了同一个集合 那么就说明无向图有环

#endif // __SPARE_GRAPH_H__