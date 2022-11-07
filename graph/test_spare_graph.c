// 2022/11/7
// zhangzhong

#include "spare_graph.h"

void test_spare_graph() {
  spare_graph_t graph;
  spare_graph_init(&graph, 8);
  spare_graph_free(&graph);
}

int main(int argc, char *argv[]) {
  test_spare_graph();
  return 0;
}