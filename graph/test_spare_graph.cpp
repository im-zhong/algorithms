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
  spare_graph_for_each(&graph, gen_graph_dot_handle);
  dot += "}\n";

  std::ofstream fout;
  fout.open("spare_graph.dot");
  fout << dot;
  fout.close();

  spare_graph_free(&graph);
}

int main(int argc, char *argv[]) {
  test_spare_graph();
  return 0;
}