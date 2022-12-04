// 2022/11/9
// zhangzhong

#ifndef __SHORTEST_PATH_H__
#define __SHORTEST_PATH_H__

#include "spare_graph.h"
#include <float.h>

// ford算法
// dijkstra
// DAG critical path

// ---------------------------------------
// 单源最短路径 Single-Source Shortest Paths
// dijkstra
// 带权重的有向图
// 路径 path = v0, v1, v2, ... , vn
// w(p) = w(v0, v1) + w(v1, v2) + ...
// 最短路径的子路径也是最短路径
// 我在完成所有基础算法之后，整理的时候
// 可以把这些重要的结论也写在头文件里面，最好可以达到
// 我忘了，我看一眼，我又想起来了。。这种程度
//

// 负数权重的边
// 如果图中存在一个环路，这个环路中存在一条负数权重的边
// 同时，从源点s可以走到这个环路
// 则此图不存在从s开始的单源最短路径
// 最短路径不能包含权重为负值的环路

// 环路
// 一条最短路径不可以包含环路，如果包含环路，去掉这个环路，会更短
// 所以可以合理的假设，所有的最短路径都是简单路径

// 所以可以实现一个斐波那契堆，因为很明显这两个算法都用到了这个结构
// 感觉挺值的
// 不过目前还是先使用二叉堆进行实现

// 1. 松弛 relaxation
// v.d: 最短路径权重 between s -> v
// 输入一条边<u, v>, 测试看看是否可以对从 s -> v的最短路径进行改善
// 通俗来讲，之前存在一条路径 s -> v
// 然后我们看看加入一个新的节点 u, 使得路径变成 s -> u -> v
// 看看新的路径是否权重更小，如果小的话，就更新相关的属性
// static inline void relax(spare_graph_t *graph, vertex_t from, vertex_t to,
//                          weight_t weight, vertex_t *pred, weight_t *distance)
//                          {
//   // source -> from -> to : distance[from] + weight
//   // source -> to : distance[to]
//   if (distance[to] > distance[from] + weight) {
//     // 更新前驱节点和距离源点的distance
//     pred[to] = from;
//     distance[to] = distance[from] + weight;
//   }
// }

// void relax_vertex(spare_graph_t *graph, vertex_t vertex, vertex_t *prev,
//                   vertex_t *distance) {
//   graph_entry_t *adjacency = NULL;
//   list_for_each_entry(adjacency, &graph->adjacency[vertex], graph_entry_t,
//                       node) {
//     relax(&graph, vertex, adjacency->vertex, adjacency->weight, prev,
//     distance);
//   }
// }

// w(u, v) >= 0
void sp_dijkstra(spare_graph_t *graph, vertex_t vertex, weight_t *distance,
                 list_node_t *spt);

// 24.2
// 在有向无环图中DAG中首先进行一次拓扑排序，然后遍历执行relax操作也可以得到sp
void sp_dag(spare_graph_t *graph, vertex_t vertex, weight_t *distance,
            list_node_t *spt);

// todo:
// 用sp_dag可以实现最长路径 其实就是把权重变负即可
// 用sp_dag可以实现关键路径 参考算法4 这里就不实现了

// 这个算法假设输入图没有负环路
void sp_bellman_ford(spare_graph_t *graph, vertex_t vertex, weight_t *distance,
                     list_node_t *spt);

#endif // __SHORTEST_PATH_H__
