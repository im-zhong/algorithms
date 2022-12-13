// 2021/9/23
// zhangzhong
// spares graph, 稀疏图，边的条数|E|远远小于|V|^2

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

#include "graph/spare_graph.h"
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

// typedef struct __spare_graph {
//   // 稀疏图实际上就是一个链表数组

//   // 每一个节点应该有一个独一无二的标识，
//   // 这个标识实际上也应该由应用程序来决定，而不是库来决定
//   // 但是这里使用序号代替 也就是long或者int
//   // 这个可以使用一个typedef来定义, 就叫...叫啥好
//   // 因为这样可以直接根数组下标进行一个交互

//   // 图的所有节点
//   // 链表数组
//   // 如果是用链表数组来表示的话 那么我们是不能随意的增加节点的
//   // 因为节点的数量在一开始就规定好了 我们能做的就是增加和减少边
//   // 作为一个学习例子来讲是足够的 而且相对来说实现也比较简单 就酱吧
//   list_node_t *adjacency;
//   size_t size;
// } spare_graph_t;

// typedef int64_t vertex_t;
// // edge
// typedef double weight_t;

// // 广度优先搜索
// // G = (V, E) 和一个源点 s
// //
// 按照广度进行搜索，也就是说，搜索完所有与s直接邻接的节点之后，才会搜索其他节点
// //

// typedef struct {
//   vertex_t vertex;
//   weight_t weight;

//   // 如果这里不需要写这个 继承队列 就好了
//   // 就更加灵活
//   // 看来还是没有必要多此一举啊，越灵活越好
//   // INHERIT_QUEUE;
//   list_node_t node;
// } graph_entry_t;

static inline graph_entry_t* make_graph_entry(vertex_t vertex,
                                              weight_t weight) {
    graph_entry_t* entry = (graph_entry_t*)malloc(sizeof(graph_entry_t));
    entry->vertex = vertex;
    entry->weight = weight;
    return entry;
}

// typedef struct {
//   vertex_t v1;
//   vertex_t v2;
//   weight_t weight;
//   list_node_t node;
// } edge_t;

edge_t* make_edge(vertex_t from, vertex_t to, weight_t weight) {
    edge_t* edge = (edge_t*)malloc(sizeof(edge_t));
    edge->v1 = from;
    edge->v2 = to;
    edge->weight = weight;
    return edge;
}

// 首先调用这个函数初始化一个图
// 然后调用 insert_edge 和 delete_edge 来改变图
// 但是图的节点数量一经初始化就不能在改变了
void spare_graph_init(spare_graph_t* graph, size_t size) {
    graph->size = size;
    graph->adjacency = (list_node_t*)malloc(size * sizeof(list_node_t));
    for (size_t i = 0; i < size; ++i) {
        list_init_head(&graph->adjacency[i]);
    }
}

void spare_graph_free(spare_graph_t* graph) {
    for (size_t i = 0; i < graph->size; ++i) {
        // list_free
    }
    // free list array
    free(graph->adjacency);
}

// 对于一个图来说 我们可以加入很多东西啊
size_t spare_graph_size(spare_graph_t* graph) { return graph->size; }

size_t spare_graph_edge(spare_graph_t* graph) {
    size_t edge = 0;
    for (size_t vertex = 0; vertex < graph->size; ++vertex) {
        // 遍历该顶点所有的邻接顶点
        list_node_t* node = NULL;
        list_for_each(node, &graph->adjacency[vertex]) { edge++; }
    }
    return edge;
}

// typedef void (*handle)(vertex_t from, vertex_t to, weight_t weight);

// 这实际上是遍历所有的边
void spare_graph_for_each_edge(spare_graph_t* graph, handle handle) {
    graph_entry_t* entry;
    for (size_t vertex = 0; vertex < graph->size; ++vertex) {
        // 遍历该顶点所有的邻接顶点
        size_t adjacency_size = 0;
        list_node_t* work = NULL;
        list_for_each(work, &graph->adjacency[vertex]) {
            entry = container_of(work, graph_entry_t, node);
            handle(vertex, entry->vertex, entry->weight);
        }
    }
}

// 我就想知道某条边的weight
weight_t spare_graph_weight(spare_graph_t* graph, vertex_t from, vertex_t to) {
    graph_entry_t* entry;
    list_node_t* work;
    list_for_each(work, &graph->adjacency[from]) {
        entry = container_of(work, graph_entry_t, node);
        if (entry->vertex == to) {
            return entry->weight;
        }
    }
    // 否则就没有这条边
    // 我们可以返回无穷大
    return DBL_MAX;
}

// 我想加一条边  我想减一条边 这都是可以的吧
void spare_graph_insert_edge(spare_graph_t* graph, vertex_t from, vertex_t to,
                             weight_t weight) {
    // 首先 from 和 to 都不应该超过 size
    assert(from < graph->size);
    assert(to < graph->size);
    // 我们的entry是 {vertex_t vertex; weight_t weight;};
    graph_entry_t* entry = make_graph_entry(to, weight);
    list_insert_before(&graph->adjacency[from], &entry->node);
}

void spare_graph_delete_edge(spare_graph_t* graph, vertex_t from, vertex_t to) {
    assert(from < graph->size);
    assert(to < graph->size);
    graph_entry_t* entry = NULL;
    list_node_t* work;
    list_for_each(work, &graph->adjacency[from]) {
        entry = container_of(work, graph_entry_t, node);
        if (entry->vertex == to) {
            // 我们找到了
            list_erase(work);
            free(entry);
            return;
        }
    }
}

// 咱们不要这个color了 实际实现是不需要的
// enum vertex_color { white, black, gray };

// typedef struct {
//   // 这里的color可以变成一个bool，其实就是有没有访问的区别
//   bool visited;
//   // prev用于生成广度优先搜索树
//   vertex_t prev;
//   // distance用于生成最短路径
//   size_t distance;
// } bfs_t;

// // vertex queue
// // 没有模板就是这么恶心
// typedef struct {
//   vertex_t vertex;
//   INHERIT_QUEUE;
// } vqueue_entry_t;

vqueue_entry_t* make_vqueue_entry(vertex_t vertex) {
    vqueue_entry_t* entry = (vqueue_entry_t*)malloc(sizeof(vqueue_entry_t));
    entry->vertex = vertex;
    return entry;
}

// breadth-first search
// bfs
// bfs会生成一个bfs 结果
// 包含最短路径和最小生成树
// 内部malloc 调用者应该free
bfs_t* bfs(spare_graph_t* graph, vertex_t vertex) {
    // 在遍历的过程中，我还需要一个数组来记录每个节点的各种属性
    // 颜色，树的指针等等
    // 在实际的环境中，我们需要根据vertex找到对应的节点
    // 这里假设我们可以直接使用下标，如果有特殊的需要
    // 直接替换这里即可
    // graph->vertex[vertex];
    // 我们可以像上面这样直接访问一个节点

    // 创建一个辅助实现bfs的数组，并初始化一下
    bfs_t* bfstree = (bfs_t*)malloc(graph->size * sizeof(bfs_t));
    for (size_t i = 0; i < graph->size; ++i) {
        bfstree[i].visited = false;
        bfstree[i].prev = -1;
        bfstree[i].distance = -1;
    }

    // 访问源点
    bfstree[vertex].visited = true;
    // -1表示该源点是广度优先搜索树的根
    bfstree[vertex].prev = -1;
    // 源点到自身的距离为0
    bfstree[vertex].distance = 0;

    // 定义一个队列
    // 将源点放到队列中
    // 必须顶一个一个struct 专门用于这个队列 卧槽是真的麻烦 真不如用C++
    queue_init(queue);
    queue_push(&queue, make_vqueue_entry(vertex));

    // 这个队列的内存管理需要我自己来动手啊,,,烦内
    // 从队列中取出节点，访问它的所有的邻接节点 将所有还没有被visited都加入队列
    // 直到队列为空
    while (!queue_is_empty(&queue)) {
        // 从队列中取出第一个节点
        vqueue_entry_t* entry = queue_top(&queue, vqueue_entry_t);
        queue_pop(&queue);

        // 我需要遍历entry->vertex对应的链表
        // 其实遍历的就是所有的邻接节点
        graph_entry_t* adjacency_entry = NULL;
        list_for_each_entry(adjacency_entry, &(graph->adjacency[entry->vertex]),
                            graph_entry_t, node) {
            // 判断，当前节点是否被遍历过了
            if (!bfstree[adjacency_entry->vertex].visited) {
                // 没有被visited就visit
                bfstree[adjacency_entry->vertex].visited = true;
                // 他的距离是当前entry.distance + 1
                bfstree[adjacency_entry->vertex].distance =
                    bfstree[entry->vertex].distance + 1;
                // 他的前驱就是当前节点
                bfstree[adjacency_entry->vertex].prev = entry->vertex;

                // 我们需要将这个节点入队，这样就可以继续遍历下去
                queue_push(&queue, make_vqueue_entry(adjacency_entry->vertex));
            }
        }

        // 释放掉这个entry
        free(entry);
    }
    // 在理解了算法导论 图22-3 之后，你就会发现，颜色是不需要的
    // 其实gray和black都可以看作一个状态，就是我们访问了这个节点
    //
    // gray的意思是，我们将来要访问这个节点，他已经在队列里面了，但是我们还没有实际去访问
    // 而黑色就是，我们实际上访问了这个节点
    // 因此这个颜色可以使用一个bool来代替
    return bfstree;
}

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
                   bfs_t* bfs) {
    if (from != to && from != bfs[to].prev) {
        bfs_path_impl(graph, from, bfs[to].prev, bfs);
    }
    printf("%ld ", to);
}

void bfs_path(spare_graph_t* graph, vertex_t from, bfs_t* bfs) {
    for (size_t v = 0; v < graph->size; ++v) {
        // 我们可能有多个连通分量 可能没有被visited到
        if (v != from && bfs[v].visited) {
            printf("bfs path: %ld ", from);
            bfs_path_impl(graph, from, v, bfs);
            printf("\n");
        }
    }
}

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
// enum graph_color { white, gray, black };

// typedef struct {
//   // 不对啊，我为什么要在这里面记录vertex
//   // 数组的下标就是vertex啊
//   // vertex_t vertex;
//   // 前驱
//   // 与广度优先搜索不同 dfs的前驱形成森林
//   vertex_t prev;
//   // 这里是两个时间戳,一个是入队的时间戳
//   // 我傻逼了，dfs哪有队列。。。
//   // 第一个时间就是visit时间，也就是第一次访问到这个节点的时间
//   // 也就是涂成灰色的时间
//   size_t visit_time;
//   // 因为是递归实现的，所以他需要回溯，第二个时间就会回溯回来的时间
//   // 每次访问一个新的节点都会使得time++
//   // 也就是变成黑色的时间
//   // 完成所有邻接节点访问的时间
//   size_t finish_time;
//   // 节点颜色
//   int color;
// } dfsforest_t;

void dfs_visit(spare_graph_t* graph, dfsforest_t* dfsforest, vertex_t vertex,
               size_t* time) {
    // 这样写是不对的 这个time必须是全局变量
    ++(*time);
    dfsforest[vertex].visit_time = *time;
    dfsforest[vertex].color = gray;

    // 访问所有的邻接节点
    graph_entry_t* adjacency_entry = NULL;
    list_for_each_entry(adjacency_entry, &graph->adjacency[vertex],
                        graph_entry_t, node) {
        if (dfsforest[adjacency_entry->vertex].color == white) {
            dfsforest[adjacency_entry->vertex].prev = vertex;
            dfs_visit(graph, dfsforest, adjacency_entry->vertex, time);
        }
        // 这里根据邻接节点的颜色可以区分不同的边
        // while: 说明树边
        // gray: back 有环
        // black: 前向边和横向边
    }

    // 所有的邻接节点都访问完成之后，递归回溯到此处
    ++(*time);
    dfsforest[vertex].color = black;
    dfsforest[vertex].finish_time = *time;
}

// depth-first search
// O(E + V)
dfsforest_t* dfs(spare_graph_t* graph) {
    // 还是准备一个数组，类似广度优先那样就可以了
    dfsforest_t* dfsforest =
        (dfsforest_t*)malloc(graph->size * sizeof(dfsforest_t));
    for (size_t vertex = 0; vertex < graph->size; ++vertex) {
        dfsforest[vertex].color = white;
        dfsforest[vertex].prev = -1;
        dfsforest[vertex].visit_time = -1;
        dfsforest[vertex].finish_time = -1;
    }

    // 初始化时间戳
    // 必须是全局的
    // 否则时间戳因为函数的栈的特性 会不对
    size_t time = 0;

    // 遍历所有的节点
    for (size_t vertex = 0; vertex < graph->size; ++vertex) {
        if (dfsforest[vertex].color == white)
            dfs_visit(graph, dfsforest, vertex, &time);
    }

    return dfsforest;
}

// typedef struct {
//   vertex_t vertex;
//   list_node_t node;
// } vlist_entry_t;

vlist_entry_t* make_vlist_entry(vertex_t vertex) {
    vlist_entry_t* entry = (vlist_entry_t*)malloc(sizeof(vlist_entry_t));
    entry->vertex = vertex;
    return entry;
}

// 这个深度优先实现起来要简单的多
// 因为不涉及队列，仅仅是一个简单的递归

// 深度优先搜索提供了树的结构价值很高的信息
// 算法导论 图22-4, 一个典型的深度优先搜索过程, 注意时间戳
// 每个节点的发现时间与完成时间

// 其实就是dfs的变种
// 就是在节点被涂成黑色的时候插入链表头部
void toposort_impl(spare_graph_t* graph, vertex_t vertex, int* color,
                   list_node_t* topo_list) {
    // 开始访问此节点
    color[vertex] = gray;

    // 递归的访问邻接节点
    graph_entry_t* adjacency_entry = NULL;
    list_for_each_entry(adjacency_entry, &graph->adjacency[vertex],
                        graph_entry_t, node) {
        // 如果邻接节点没有被访问
        if (color[adjacency_entry->vertex] == white) {
            toposort_impl(graph, adjacency_entry->vertex, color, topo_list);
        }
        // 这里根据邻接节点的颜色可以区分不同的边
        // while: 说明树边
        // gray: back 有环
        // black: 前向边和横向边
    }

    // 此节点访问完成
    color[vertex] = black;
    // 插入链表头
    vlist_entry_t* entry = make_vlist_entry(vertex);
    list_insert_after(topo_list, &entry->node);
}

// 拓扑排序
void toposort(spare_graph_t* graph, list_node_t* topo_list) {
    // 拓扑排序非常简单
    // call dfs
    // 每当一个节点被涂成黑色时 也就是 finish visit时
    // 插入一个链表的头部
    // 最后从前到后输出链表就是拓扑排序

    int* color = (int*)malloc(graph->size * sizeof(int));
    for (size_t i = 0; i < graph->size; ++i) {
        color[i] = white;
    }

    // 遍历所有的链表节点
    for (size_t vertex = 0; vertex < graph->size; ++vertex) {
        if (color[vertex] == white)
            toposort_impl(graph, vertex, color, topo_list);
    }
}

bool spare_graph_has_circle_impl(spare_graph_t* graph, vertex_t vertex,
                                 int* color) {
    color[vertex] = gray;

    // 递归的访问邻接节点
    graph_entry_t* adjacency_entry = NULL;
    list_for_each_entry(adjacency_entry, &graph->adjacency[vertex],
                        graph_entry_t, node) {
        // 如果邻接节点没有被访问
        if (color[adjacency_entry->vertex] == white) {
            if (spare_graph_has_circle_impl(graph, adjacency_entry->vertex,
                                            color)) {
                return true;
            }
        } else if (color[adjacency_entry->vertex] == gray) {
            return true;
        }
        // 这里根据邻接节点的颜色可以区分不同的边
        // while: 说明树边
        // gray: back 有环
        // black: 前向边和横向边
    }

    color[vertex] = black;
    return false;
}

// 环路判断非常简单 同样也是用dfs实现
// 只要碰到一条back path 就直接报告有环路
// 一个稍微复杂一点的问题是 报告环路路径
// 我们需要记录每个节点的dfs搜索前驱 这样碰到环路是 就可以向上搜索 从而给出环路
bool spare_graph_has_circle(spare_graph_t* graph) {
    int* color = (int*)malloc(graph->size * sizeof(int));
    for (size_t i = 0; i < graph->size; ++i) {
        color[i] = white;
    }

    for (size_t vertex = 0; vertex < graph->size; ++vertex) {
        if (color[vertex] == white) {
            if (spare_graph_has_circle_impl(graph, vertex, color)) {
                return true;
            }
        }
    }
    return false;
}

// 用于mst spt算法
bool weight_less(const void* lhs, const void* rhs) {
    return *(weight_t*)lhs < *(weight_t*)rhs;
}

// #endif // __SPARE_GRAPH_H__