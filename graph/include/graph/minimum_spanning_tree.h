// 2022/11/9
// zhangzhong

#ifndef __MINIMUM_SPANNING_TREE_H__
#define __MINIMUM_SPANNING_TREE_H__

#include "spare_graph.h"

// kruskal
// prim
// -------------------------------------------
// 这些实现应该放在一个单独的.c文件里面
// 最小生成树 mst
// minimal spanning tree
// 如果采取kruskal实现的话，我可以使用disjoint_set
// 而且最终的最小生成树应该就是一个disjoint_set
// 重要的提示：这两个最小生成树算法都是无向连通图的

// list_node_t edge_list;
// void mst_kruskal_handle(vertex_t from, vertex_t to, weight_t weight);

// 返回一个edge list head 最小生成树是边的集合
void mst_kruskal(spare_graph_t *graph, list_node_t *mst);

// 这个实现是真的简单，就是感觉不太适合邻接链表

// prim算法依赖一个最小堆
// #include "heap.c"

// 这个算法也非常的简单
// 有两个集合
// 一个是 mst
// 另一个是图的点集V
// 两者是不相交的
// 算法维护一个最小堆:
// 保存的是V中顶点，排序的依据是V顶点到mst中任意顶点的最短边权重
// 每次从堆中取出最小的顶点，从V中删除，加入mst中
// 同时更新此节点的所有邻接节点的权重，如果变小了的话
// 重复这个过程 直到V变成空集

void mst_prim(spare_graph_t *graph, list_node_t *mst);

// 这个算法理解起来就比kruskal要稍微复杂一些
// 然后也需要很多额外的数组来保存一些中间信息
// 还是算了，不实现了
// 只要领会了思想就可以了

// 还有一个问题，就是验证是否是最小生成树的问题
// 在一篇论文中有所描述

#endif // __MINIMUM_SPANNING_TREE_H__