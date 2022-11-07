// 2022/11/7
// zhangzhong

#include "spare_graph.h"
#include <cassert>
#include <fstream>
#include <string>

std::string dot;
void gen_graph_dot_handle(vertex_t from, vertex_t to, weight_t weight) {
  dot += ("  " + std::to_string(from) + " -> " + std::to_string(to)) + "\n";
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

int main(int argc, char *argv[]) {
  test_spare_graph();
  return 0;
}