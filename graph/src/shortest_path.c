// 2022/11/9
// zhangzhong

#include "graph/shortest_path.h"
#include "graph/spare_graph.h"
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
void sp_dijkstra(spare_graph_t* graph, vertex_t vertex, weight_t* distance,
                 list_node_t* spt) {
    // 和prim算法类似 定义一个prev数组表示他的前驱
    vertex_t* prev = (vertex_t*)malloc(graph->size * sizeof(vertex_t));
    for (size_t i = 0; i < graph->size; ++i) {
        prev[i] = i;
    }
    // 定义一个distance数组表示到达源点的距离
    // 可以让用户提供 同时也可以作为返回数据
    // weight_t *distance = (weight_t *)malloc(graph->size * sizeof(weight_t));
    for (size_t i = 0; i < graph->size; ++i) {
        distance[i] = DBL_MAX;
    }
    // 定义一个数组表示节点是否加入最短路径树了
    //   bool *in_spt = (bool *)malloc(graph->size * sizeof(bool));
    //   for (size_t i = 0; i < graph->size; ++i) {
    //     in_spt[i] = false;
    //   }
    weight_t* prev_weight = (weight_t*)malloc(graph->size * sizeof(weight_t));
    for (size_t i = 0; i < graph->size; ++i) {
        prev_weight[i] = DBL_MAX;
    }

    // 类似prim dijkstra也使用一个堆来维护加入到spt中的下一个节点
    iheap_t heap;
    iheap_init(&heap, distance, graph->size, sizeof(weight_t), graph->size + 1,
               weight_less);

    // 源点到自身的距离自然是0
    distance[vertex] = 0;
    prev_weight[vertex] = 0;
    // 将源点放入堆中
    iheap_push(&heap, vertex);
    while (!iheap_is_empty(&heap)) {
        vertex_t v = iheap_top(&heap);
        iheap_pop(&heap);

        // 将此节点加入spt
        // 将此边加入spt
        // prev(v)->v: 但是权重是多少呢?? distance保存的并不是权重 而是总的距离
        // 这种结构适合给出最短路径的权重值 但是不适合给出每条边上的权重
        // 和prim算法相比 dijkstra不需要这个数据结构 因为碰到这种back edge
        // relax操作会正确的处理
        // in_spt[v] = true;

        // 遍历v所有的邻接节点
        // 卧槽 处理方式和prim简直一摸一样
        graph_entry_t* adjacency = NULL;
        list_for_each_entry(adjacency, &graph->adjacency[v], graph_entry_t,
                            node) {
            vertex_t from = v;
            vertex_t to = adjacency->vertex;
            weight_t weight = adjacency->weight;
            // source -> from -> to : distance[from] + weight
            // source -> to : distance[to]
            if (distance[to] > distance[from] + weight) {
                // 更新前驱节点和距离源点的distance
                prev[to] = from;
                distance[to] = distance[from] + weight;
                prev_weight[to] = weight;

                // 如果更新了distance和pred
                // 那么相应的需要更新heap
                if (!iheap_contain(&heap, to)) {
                    iheap_push(&heap, to);
                } else {
                    iheap_update(&heap, to);
                }
            }
        }
    }

    for (size_t i = 0; i < graph->size; ++i) {
        // edge: pred[i] -> i： weight[i]
        if (prev[i] != i) {
            edge_t* edge = make_edge(prev[i], i, prev_weight[i]);
            list_insert_after(spt, &edge->node);
        }
    }
}

// 24.2
// 在有向无环图中DAG中首先进行一次拓扑排序，然后遍历执行relax操作也可以得到sp
void sp_dag(spare_graph_t* graph, vertex_t vertex, weight_t* distance,
            list_node_t* spt) {
    // toposort应该返回一个链表
    vertex_t* prev = (vertex_t*)malloc(graph->size * sizeof(vertex_t));
    for (size_t i = 0; i < graph->size; ++i) {
        prev[i] = i;
    }
    // 定义一个distance数组表示到达源点的距离
    // weight_t *distance = (weight_t *)malloc(graph->size * sizeof(weight_t));
    for (size_t i = 0; i < graph->size; ++i) {
        distance[i] = DBL_MAX;
    }
    // 最终根据pred和distance生成spt
    // 如果要返回带权重的最短路径图 也不是不行
    // 我们额外还要一个数组 用来保存当前前驱所对应的边的权重
    weight_t* prev_weight = (weight_t*)malloc(graph->size * sizeof(weight_t));
    for (size_t i = 0; i < graph->size; ++i) {
        prev_weight[i] = DBL_MAX;
    }

    list_node_t topo_list;
    list_init_head(&topo_list);
    toposort(graph, &topo_list);
    // for each vlist
    // 然后对每一个vertex做relax操作
    // 就可以在线性时间内拿到spt
    vlist_entry_t* entry = NULL;
    list_for_each_entry(entry, &topo_list, vlist_entry_t, node) {
        vertex_t v = entry->vertex;
        // 对于每一个节点执行一次relax操作
        graph_entry_t* adjacency = NULL;
        list_for_each_entry(adjacency, &graph->adjacency[v], graph_entry_t,
                            node) {
            vertex_t from = v;
            vertex_t to = adjacency->vertex;
            weight_t weight = adjacency->weight;
            // source -> from -> to : distance[from] + weight
            // source -> to : distance[to]
            if (distance[to] > distance[from] + weight) {
                // 更新前驱节点和距离源点的distance
                prev[to] = from;
                distance[to] = distance[from] + weight;
                prev_weight[to] = weight;
            }
        }
    }

    // 最终根据pred和distance生成spt
    // 如果要返回带权重的最短路径图 也不是不行
    // 我们额外还要一个数组 用来保存当前前驱所对应的边的权重
    for (size_t i = 0; i < graph->size; ++i) {
        // edge: pred[i] -> i： weight[i]
        if (prev[i] != i) {
            edge_t* edge = make_edge(prev[i], i, prev_weight[i]);
            list_insert_after(spt, &edge->node);
        }
    }
    // 但是distance数组怎么返回呢??
}

// todo:
// 用sp_dag可以实现最长路径 其实就是把权重变负即可
// 用sp_dag可以实现关键路径 参考算法4 这里就不实现了

// 这个算法假设输入图没有负环路
void sp_bellman_ford(spare_graph_t* graph, vertex_t vertex, weight_t* distance,
                     list_node_t* spt) {
    // 就像广度优先搜索那样 使用一个数组来保存接下来需要执行relax操作的节点
    // 直到数组为空
    // 而接下来需要执行relax操作的节点就是刚刚更新了distance数组的节点
    // 并且不在queue中的节点
    vertex_t* prev = (vertex_t*)malloc(graph->size * sizeof(vertex_t));
    for (size_t i = 0; i < graph->size; ++i) {
        prev[i] = i;
    }
    // 定义一个distance数组表示到达源点的距离
    // weight_t *distance = (weight_t *)malloc(graph->size * sizeof(weight_t));
    for (size_t i = 0; i < graph->size; ++i) {
        distance[i] = DBL_MAX;
    }
    bool* in_queue = (bool*)malloc(graph->size * sizeof(bool));
    for (size_t i = 0; i < graph->size; ++i) {
        in_queue[i] = false;
    }
    weight_t* prev_weight = (weight_t*)malloc(graph->size * sizeof(weight_t));
    for (size_t i = 0; i < graph->size; ++i) {
        prev_weight[i] = DBL_MAX;
    }

    // 还需要一个queue
    // 用来保存vertex
    queue_init(queue);
    // 在最开始 将源节点放入队列中
    prev[vertex] = vertex;
    distance[vertex] = 0;
    prev_weight[vertex] = 0;
    queue_push(&queue, make_vqueue_entry(vertex));
    in_queue[vertex] = true;

    while (!queue_is_empty(&queue)) {
        // 从队列中取出一个节点
        vqueue_entry_t* entry = queue_top(&queue, vqueue_entry_t);
        queue_pop(&queue);
        in_queue[vertex] = false;

        // 遍历邻接节点
        graph_entry_t* adjacency_entry = NULL;
        list_for_each_entry(adjacency_entry, &(graph->adjacency[entry->vertex]),
                            graph_entry_t, node) {
            vertex_t from = entry->vertex;
            vertex_t to = adjacency_entry->vertex;
            weight_t weight = adjacency_entry->weight;
            // 执行relax的逻辑
            // source -> from -> to : distance[from] + weight
            // source -> to : distance[to]
            if (distance[to] > distance[from] + weight) {
                // 更新前驱节点和距离源点的distance
                prev[to] = from;
                distance[to] = distance[from] + weight;
                prev_weight[to] = weight;

                // 如果更新了distance 那么需要加入队列中
                if (!in_queue[to]) {
                    queue_push(&queue, make_vqueue_entry(to));
                    in_queue[to] = true;
                }
            }
        }
    }

    for (size_t i = 0; i < graph->size; ++i) {
        // edge: pred[i] -> i： weight[i]
        if (prev[i] != i) {
            edge_t* edge = make_edge(prev[i], i, prev_weight[i]);
            list_insert_after(spt, &edge->node);
        }
    }
}
