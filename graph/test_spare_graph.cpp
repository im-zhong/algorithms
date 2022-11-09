// 2022/11/7
// zhangzhong

#include "minimum_spanning_tree.h"
#include "shortest_path.h"
#include "spare_graph.h"
#include <cassert>
#include <fstream>
#include <string>

std::string dot;
void gen_graph_dot_handle(vertex_t from, vertex_t to, weight_t weight) {
  dot += (" " + std::to_string(from) + " -> " + std::to_string(to) +
          " [label=\"" + std::to_string(weight) + "\"]\n");
}

void gen_spare_graph_dot(spare_graph_t *graph) {
  // 遍历整张图
  dot.clear();
  dot += "digraph {\n";
  spare_graph_for_each_edge(graph, gen_graph_dot_handle);
  dot += "}\n";

  std::ofstream fout;
  fout.open("spare_graph.dot");
  fout << dot;
  fout.close();
}

// [vertex visit_time/finish_time]
// 这里需要一个辅助函数 提供dfsforest 和 vertex
// 你给我生成上面这个东西
std::string dfsforest_node(dfsforest_t *dfs_forest, vertex_t vertex) {
  return "\"" + std::to_string(vertex) + " " +
         std::to_string(dfs_forest[vertex].visit_time) + "/" +
         std::to_string(dfs_forest[vertex].finish_time) + "\"";
}

void test_spare_graph() {
  spare_graph_t graph;
  spare_graph_init(&graph, 8);

  spare_graph_insert_edge(&graph, 0, 1, 1);
  spare_graph_insert_edge(&graph, 0, 2, 1);
  spare_graph_insert_edge(&graph, 0, 3, 1);
  spare_graph_insert_edge(&graph, 1, 2, 1);
  spare_graph_insert_edge(&graph, 1, 4, 1);
  spare_graph_insert_edge(&graph, 2, 5, 1);
  spare_graph_insert_edge(&graph, 2, 6, 1);
  spare_graph_insert_edge(&graph, 3, 6, 1);
  spare_graph_insert_edge(&graph, 6, 7, 1);
  assert(spare_graph_edge(&graph) == 9);

  spare_graph_delete_edge(&graph, 0, 1);
  spare_graph_delete_edge(&graph, 3, 6);
  assert(spare_graph_edge(&graph) == 7);

  // 遍历整张图
  dot.clear();
  dot += "digraph {\n";
  spare_graph_for_each_edge(&graph, gen_graph_dot_handle);
  dot += "}\n";

  std::ofstream fout;
  fout.open("spare_graph.dot");
  fout << dot;
  fout.close();

  // test bfs
  vertex_t source = 0;
  bfs_t *bfstree = bfs(&graph, source);
  bfs_path(&graph, source, bfstree);

  // 生成广度优先树
  // 非常简单 只需要遍历一遍bfstree 把所有visited的前驱打印一下就可以了
  dot.clear();
  dot += "digraph {\n";
  for (size_t v = 0; v < graph.size; ++v) {
    if (bfstree[v].visited && bfstree[v].prev >= 0) {
      dot += ("  " + std::to_string(bfstree[v].prev) + " -> " +
              std::to_string(v) + "\n");
    }
  }
  dot += "}\n";

  fout.open("bfstree.dot");
  fout << dot;
  fout.close();

  // test dfs
  dfsforest_t *dfsforest = dfs(&graph);
  dot.clear();
  dot += "digraph {\n";
  for (size_t v = 0; v < graph.size; ++v) {
    // 深度优先搜索会访问所有节点
    assert(dfsforest[v].color == black);
    if (dfsforest[v].prev != -1) {
      // from -> to
      // prev(v) -> v
      dot += "  " + dfsforest_node(dfsforest, dfsforest[v].prev) + " -> " +
             dfsforest_node(dfsforest, v) + "\n";
    }
  }
  dot += "}\n";
  fout.open("dfsforest.dot");
  fout << dot;
  fout.close();

  free(dfsforest);
  free(bfstree);
  spare_graph_free(&graph);
}

void test_mst() {

  std::ofstream fout;

  spare_graph_t graph;
  spare_graph_init(&graph, 8);

  // 说明一点
  // 虽然这里是有向图 但是我们的算法内部会生成一个无向图
  // 就是将单项的边看成双向的边
  // 然后这个weight就得随机一下了
  spare_graph_insert_edge(&graph, 0, 1, 8);
  spare_graph_insert_edge(&graph, 0, 2, 9);
  spare_graph_insert_edge(&graph, 0, 3, 3);
  spare_graph_insert_edge(&graph, 1, 2, 10);
  spare_graph_insert_edge(&graph, 1, 4, 4);
  spare_graph_insert_edge(&graph, 2, 5, 7);
  spare_graph_insert_edge(&graph, 2, 6, 6);
  spare_graph_insert_edge(&graph, 3, 6, 2);
  spare_graph_insert_edge(&graph, 6, 7, 5);
  assert(spare_graph_edge(&graph) == 9);
  gen_spare_graph_dot(&graph);

  list_node_t mst;
  list_init_head(&mst);
  // 测试 最小生成树
  mst_kruskal(&graph, &mst);
  edge_t *edge = NULL;
  dot.clear();
  // 这次是无向图了
  // a -- b [label = "lable"]
  dot += "graph {\n";
  list_for_each_entry(edge, &mst, edge_t, node) {
    dot += (" " + std::to_string(edge->v1) + " -- " + std::to_string(edge->v2) +
            " [label=\"" + std::to_string(edge->weight) + "\"]\n");
  }
  dot += "}\n";

  fout.open("mst_kruskal.dot");
  fout << dot;
  fout.close();

  assert(spare_graph_edge(&graph) == 9);
  gen_spare_graph_dot(&graph);

  // 因为这两个最小生成树的算法都是给连通无向图设计的
  // 而邻接链表更适合有向图
  // 但是修正的方法也非常简单
  // 只需要将反向的边添加进去即可
  spare_graph_insert_edge(&graph, 1, 0, 8);
  spare_graph_insert_edge(&graph, 2, 0, 9);
  spare_graph_insert_edge(&graph, 3, 0, 3);
  spare_graph_insert_edge(&graph, 2, 1, 10);
  spare_graph_insert_edge(&graph, 4, 1, 4);
  spare_graph_insert_edge(&graph, 5, 2, 7);
  spare_graph_insert_edge(&graph, 6, 2, 6);
  spare_graph_insert_edge(&graph, 6, 3, 2);
  spare_graph_insert_edge(&graph, 7, 6, 5);

  list_node_t prim_mst;
  list_init_head(&prim_mst);
  // 测试 最小生成树
  mst_prim(&graph, &prim_mst);
  edge = NULL;
  dot.clear();
  // 这次是无向图了
  // a -- b [label = "lable"]
  dot += "graph {\n";
  list_for_each_entry(edge, &prim_mst, edge_t, node) {
    dot += (" " + std::to_string(edge->v1) + " -- " + std::to_string(edge->v2) +
            " [label=\"" + std::to_string(edge->weight) + "\"]\n");
  }
  dot += "}\n";

  fout.open("mst_prim.dot");
  fout << dot;
  fout.close();

  // todo: free list
  spare_graph_free(&graph);
}

// 根据单源最短路返回的edge list 生成dot
void gen_sp_dot(list_node_t *spt, std::string name) {
  std::string dot;
  edge_t *edge = NULL;
  dot += "digraph {\n";
  list_for_each_entry(edge, spt, edge_t, node) {
    dot += (" " + std::to_string(edge->v1) + " -> " + std::to_string(edge->v2) +
            " [label=\"" + std::to_string(edge->weight) + "\"]\n");
  }
  dot += "}\n";

  std::ofstream fout;
  fout.open(name);
  fout << dot;
  fout.close();
}

void test_sp() {
  spare_graph_t graph;
  spare_graph_init(&graph, 8);

  // 说明一点
  // 虽然这里是有向图 但是我们的算法内部会生成一个无向图
  // 就是将单项的边看成双向的边
  // 然后这个weight就得随机一下了
  spare_graph_insert_edge(&graph, 0, 1, 8);
  spare_graph_insert_edge(&graph, 0, 2, 9);
  spare_graph_insert_edge(&graph, 0, 3, 3);
  spare_graph_insert_edge(&graph, 1, 2, 10);
  spare_graph_insert_edge(&graph, 1, 4, 4);
  spare_graph_insert_edge(&graph, 2, 5, 7);
  spare_graph_insert_edge(&graph, 2, 6, 6);
  spare_graph_insert_edge(&graph, 3, 6, 2);
  spare_graph_insert_edge(&graph, 6, 7, 5);
  assert(spare_graph_edge(&graph) == 9);
  gen_spare_graph_dot(&graph);

  list_node_t dijkstra_spt;
  list_init_head(&dijkstra_spt);
  weight_t *dijkstra_distance =
      (weight_t *)malloc(graph.size * sizeof(weight_t));
  sp_dijkstra(&graph, 0, dijkstra_distance, &dijkstra_spt);
  gen_sp_dot(&dijkstra_spt, "dijkstra.dot");

  list_node_t dag_spt;
  list_init_head(&dag_spt);
  weight_t *dag_distance = (weight_t *)malloc(graph.size * sizeof(weight_t));
  sp_dijkstra(&graph, 0, dag_distance, &dag_spt);
  gen_sp_dot(&dag_spt, "dag_spt.dot");

  list_node_t bellman_ford_spt;
  list_init_head(&bellman_ford_spt);
  weight_t *bellman_ford_distance =
      (weight_t *)malloc(graph.size * sizeof(weight_t));
  sp_dijkstra(&graph, 0, bellman_ford_distance, &bellman_ford_spt);
  gen_sp_dot(&bellman_ford_spt, "bellman_ford.dot");

  free(dag_distance);
  free(dijkstra_distance);
  spare_graph_free(&graph);
}

void test_has_circle() {
  spare_graph_t graph;
  spare_graph_init(&graph, 8);

  // 说明一点
  // 虽然这里是有向图 但是我们的算法内部会生成一个无向图
  // 就是将单项的边看成双向的边
  // 然后这个weight就得随机一下了
  spare_graph_insert_edge(&graph, 0, 1, 8);
  spare_graph_insert_edge(&graph, 0, 2, 9);
  spare_graph_insert_edge(&graph, 0, 3, 3);
  spare_graph_insert_edge(&graph, 1, 2, 10);
  spare_graph_insert_edge(&graph, 1, 4, 4);
  spare_graph_insert_edge(&graph, 2, 5, 7);
  spare_graph_insert_edge(&graph, 2, 6, 6);
  spare_graph_insert_edge(&graph, 3, 6, 2);
  spare_graph_insert_edge(&graph, 6, 7, 5);
  assert(spare_graph_edge(&graph) == 9);
  assert(!spare_graph_has_circle(&graph));

  spare_graph_insert_edge(&graph, 6, 3, 2);
  assert(spare_graph_has_circle(&graph));
}

// todo: random test

int main(int argc, char *argv[]) {
  test_spare_graph();
  test_mst();
  test_sp();
  test_has_circle();
  return 0;
}