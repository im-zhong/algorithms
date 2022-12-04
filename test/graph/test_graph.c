// 2022/11/7
// zhangzhong

#include "graph/minimum_spanning_tree.h"
#include "graph/shortest_path.h"
#include "graph/spare_graph.h"
#include "util/c_string.h"
#include <assert.h>
#include <stdio.h>

string_t dot;
void gen_graph_dot_handle(vertex_t from, vertex_t to, weight_t weight) {
  // dot += (" " + std::to_string(from) + " -> " + std::to_string(to) +
  //         " [label=\"" + std::to_string(weight) + "\"]\n");
  string_append(&dot, " %ld -> %ld [label=\"%g\"]\n", from, to, weight);
}

void gen_spare_graph_dot(spare_graph_t *graph) {
  // 遍历整张图
  // dot.clear();
  // dot += "digraph {\n";
  // spare_graph_for_each_edge(graph, gen_graph_dot_handle);
  // dot += "}\n";

  // std::ofstream fout;
  // fout.open("spare_graph.dot");
  // fout << dot;
  // fout.close();

  string_clear(&dot);
  string_append(&dot, "digraph {\n");
  spare_graph_for_each_edge(graph, gen_graph_dot_handle);
  string_append(&dot, "}\n");

  FILE *fp = fopen("spare_graph.dot", "w");
  assert(fp);
  fprintf(fp, "%s\n", dot.data);
  fclose(fp);
}

void gen_bfs_dot(spare_graph_t *graph, bfs_t *bfstree) {
  string_t dot = make_string("");
  string_append(&dot, "digraph {\n");
  for (size_t v = 0; v < graph->size; ++v) {
    if (bfstree[v].visited && bfstree[v].prev >= 0) {
      // dot += ("  " + std::to_string(bfstree[v].prev) + " -> " +
      //         std::to_string(v) + "\n");
      string_append(&dot, "  %ld -> %ld\n", bfstree[v].prev, v);
    }
  }
  string_append(&dot, "}\n");

  FILE *fp = fopen("bfstree.dot", "w");
  assert(fp);
  fprintf(fp, "%s\n", dot.data);
  fclose(fp);
  free_string(&dot);
}

// [vertex visit_time/finish_time]
// 这里需要一个辅助函数 提供dfsforest 和 vertex
// 你给我生成上面这个东西
string_t dfsforest_node(dfsforest_t *dfs_forest, vertex_t vertex) {
  // return "\"" + std::to_string(vertex) + " " +
  //        std::to_string(dfs_forest[vertex].visit_time) + "/" +
  //        std::to_string(dfs_forest[vertex].finish_time) + "\"";
  return make_string("\"%ld %ld/%ld\"", vertex, dfs_forest[vertex].visit_time,
                     dfs_forest[vertex].finish_time);
}

void gen_dfs_dot(spare_graph_t *graph, dfsforest_t *dfsforest) {
  string_t dot = make_string("");
  string_append(&dot, "digraph {\n");
  for (size_t v = 0; v < graph->size; ++v) {
    // 深度优先搜索会访问所有节点
    assert(dfsforest[v].color == black);
    if (dfsforest[v].prev != -1) {
      // from -> to
      // prev(v) -> v
      // dot += "  " + dfsforest_node(dfsforest, dfsforest[v].prev) + " -> " +
      //        dfsforest_node(dfsforest, v) + "\n";
      string_t prev = dfsforest_node(dfsforest, dfsforest[v].prev);
      string_t next = dfsforest_node(dfsforest, v);
      string_append(&dot, "  %s -> %s\n", prev.data, next.data);
      free_string(&prev);
      free_string(&next);
    }
  }
  string_append(&dot, "}\n");

  FILE *fp = fopen("dfsforest.dot", "w");
  assert(fp);
  fprintf(fp, "%s\n", dot.data);
  fclose(fp);

  free_string(&dot);
}

void gen_mst_dot(list_node_t *mst, const char *filename) {
  edge_t *edge = NULL;

  string_t dot = make_string("");
  // 这次是无向图了
  // a -- b [label = "lable"]
  string_append(&dot, "graph {\n");
  list_for_each_entry(edge, mst, edge_t, node) {
    // dot += (" " + std::to_string(edge->v1) + " -- " +
    // std::to_string(edge->v2) +
    //         " [label=\"" + std::to_string(edge->weight) + "\"]\n");
    string_append(&dot, " %ld -- %ld [label=\"%g\"]\n", edge->v1, edge->v2,
                  edge->weight);
  }
  string_append(&dot, "}\n");

  FILE *fp = fopen(filename, "w");
  assert(fp);
  fprintf(fp, "%s\n", dot.data);
  fclose(fp);

  free_string(&dot);
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
  gen_spare_graph_dot(&graph);

  // test bfs
  vertex_t source = 0;
  bfs_t *bfstree = bfs(&graph, source);
  bfs_path(&graph, source, bfstree);

  // 生成广度优先树
  // 非常简单 只需要遍历一遍bfstree 把所有visited的前驱打印一下就可以了
  gen_bfs_dot(&graph, bfstree);

  // test dfs
  dfsforest_t *dfsforest = dfs(&graph);
  gen_dfs_dot(&graph, dfsforest);

  free(dfsforest);
  free(bfstree);
  spare_graph_free(&graph);
}

void test_mst() {
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

  list_node_t kruskal_mst;
  list_init_head(&kruskal_mst);
  // 测试 最小生成树
  mst_kruskal(&graph, &kruskal_mst);

  gen_mst_dot(&kruskal_mst, "mst_kruskal.dot");

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
  gen_mst_dot(&prim_mst, "mst_prim.dot");

  // todo: free list
  spare_graph_free(&graph);
}

// 根据单源最短路返回的edge list 生成dot
void gen_sp_dot(list_node_t *sp, const char *filename) {
  string_t dot = make_string("");
  edge_t *edge = NULL;
  string_append(&dot, "digraph {\n");
  list_for_each_entry(edge, sp, edge_t, node) {
    // dot += (" " + std::to_string(edge->v1) + " -> " +
    // std::to_string(edge->v2) +
    //         " [label=\"" + std::to_string(edge->weight) + "\"]\n");
    string_append(&dot, " %ld -> %ld [label=\"%g\"]\n", edge->v1, edge->v2,
                  edge->weight);
  }
  string_append(&dot, "}\n");

  FILE *fp = fopen(filename, "w");
  assert(fp);
  fprintf(fp, "%s\n", dot.data);
  fclose(fp);

  free_string(&dot);
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
