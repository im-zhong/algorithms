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

list_node_t edge_list;
void mst_kruskal_handle(vertex_t from, vertex_t to, weight_t weight) {
  list_node_t *work = NULL;
  edge_t *edge = NULL;
  list_for_each(work, &edge_list) {
    edge = container_of(work, edge_t, node);
    if (edge->weight >= weight) {
      break;
    }
  }
  edge = make_edge(from, to, weight);
  list_insert_before(work, &edge->node);
}

// 返回一个edge list head 最小生成树是边的集合
void mst_kruskal(spare_graph_t *graph, list_node_t *mst) {
  // 声明一个空的MST集合
  // 这个东西应该是一个edge的集合

  // 首先制造一个disjoint_set,
  // 此时，每个节点都是独立的
  disjoint_set djs;
  disjoint_set_init(&djs, graph->size);

  // sort the edges of G.E by weight in decreasing order
  // 我要按照weight进行排序
  // 然后根据排序结果, 按照顺序遍历
  // 这要怎么实现呢？？
  // 我要维护一个边的数组？？
  // 这一部分实现起来应该也是比较简单, 准备一个数组，
  // 存储所有的边，然后进行一个快速排序，即可
  // 假设我这里已经获得了一个这样的数组
  list_init_head(&edge_list);
  spare_graph_for_each_edge(graph, mst_kruskal_handle);

  edge_t *edge = NULL;
  list_node_t *work = NULL;
  list_for_each_entry(edge, &edge_list, edge_t, node) {
    if (disjoint_set_find(&djs, edge->v1) !=
        disjoint_set_find(&djs, edge->v2)) {
      // 最小生成树是边的集合
      // MST = MST union {edge(v1, v2)}
      edge_t *safe_edge = make_edge(edge->v1, edge->v2, edge->weight);
      list_insert_after(mst, &safe_edge->node);
      disjoint_set_union(&djs, edge->v1, edge->v2);
    }
  }

  // todo: free_list
  // 最终可以返回MST
  // 只能用一个链表来表示MST
}

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

void mst_prim(spare_graph_t *graph, list_node_t *mst) {
  if (graph->size == 0) {
    return;
  }

  // 全部重写
  // mst使用一个index binary heap 保存V中的节点到A中的节点的最小权重
  weight_t *weights = (weight_t *)malloc(graph->size * sizeof(weight_t));
  // 在一开始没有节点在A中 所以所有的权重都是无穷大
  for (size_t i = 0; i < graph->size; ++i) {
    weights[i] = DBL_MAX;
  }

  // 还要有一个数组 用来保存那个节点已经放到mst中了
  bool *in_mst = (bool *)malloc(graph->size * sizeof(bool));
  for (size_t i = 0; i < graph->size; ++i) {
    // 初始时 所有节点都不在mst中
    in_mst[i] = false;
  }

  // 还需要一个数组 用来表示达成最小权重的前驱是谁
  // 指向它自己表示没有前驱
  vertex_t *pred = (vertex_t *)malloc(graph->size * sizeof(vertex_t));
  for (size_t i = 0; i < graph->size; ++i) {
    pred[i] = i;
  }

  // 初始化 iheap
  iheap_t heap;
  iheap_init(&heap, weights, graph->size, sizeof(weight_t), graph->size + 1,
             weight_less);

  // 向heap中插入初始节点
  weights[0] = 0;
  iheap_push(&heap, 0);
  // 现在节点0在mst中le
  in_mst[0] = true;

  while (!iheap_is_empty(&heap)) {
    // 从这里知道的边呀
    vertex_t vertex = iheap_top(&heap);
    iheap_pop(&heap);
    // 将此边加入mst
    // [pred[vertex], vertex]: weights[vertex]
    // MST = MST union {edge(v1, v2)}
    if (vertex != 0) {
      edge_t *safe_edge = make_edge(pred[vertex], vertex, weights[vertex]);
      list_insert_after(mst, &safe_edge->node);
    }

    in_mst[vertex] = true;
    // 然后我们遍历此节点所有的邻接节点
    // 如果这个节点不在mst中 则加入heap中
    // 如果heap包含此邻接节点 还需要检查权重是否变小
    // 如果变小 则 update
    graph_entry_t *adjacency = NULL;
    list_for_each_entry(adjacency, &graph->adjacency[vertex], graph_entry_t,
                        node) {
      if (!in_mst[adjacency->vertex]) {
        if (iheap_contain(&heap, adjacency->vertex)) {
          if (weights[adjacency->vertex] > adjacency->weight) {
            // 更新权重 然后更新heap
            weights[adjacency->vertex] = adjacency->weight;
            pred[adjacency->vertex] = vertex;
            iheap_update(&heap, adjacency->vertex);
          }
        } else {
          weights[adjacency->vertex] = adjacency->weight;
          pred[adjacency->vertex] = vertex;
          iheap_push(&heap, adjacency->vertex);
        }
      }
    }
  }
}

// 这个算法理解起来就比kruskal要稍微复杂一些
// 然后也需要很多额外的数组来保存一些中间信息
// 还是算了，不实现了
// 只要领会了思想就可以了

// 还有一个问题，就是验证是否是最小生成树的问题
// 在一篇论文中有所描述

#endif // __MINIMUM_SPANNING_TREE_H__