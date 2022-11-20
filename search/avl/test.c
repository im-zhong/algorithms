// 2022/11/19
// zhangzhong
// avl tree

#include "avl.h"
#include <assert.h>
#include <container/c_random.h>
#include <container/c_string.h>
#include <container/set.h>
#include <stdio.h>

string_t tree_node_to_string(tree_node_t *root) {
  return make_string("%ld:%u", root->key, root->height);
}

void gen_avl_dot_impl(tree_node_t *root, string_t *dot) {
  if (!root) {
    return;
  }

  string_t root_name = tree_node_to_string(root);
  string_append(dot, "  \"%s\"\n", root_name.data);
  if (root->left) {
    string_t left_name = tree_node_to_string(root->left);
    string_append(dot, "  \"%s\" -> \"%s\"\n", root_name.data, left_name.data);
    free_string(&left_name);
  }
  if (root->right) {
    string_t right_name = tree_node_to_string(root->right);
    string_append(dot, "  \"%s\" -> \"%s\"\n", root_name.data, right_name.data);
    free_string(&right_name);
  }
  free_string(&root_name);

  gen_avl_dot_impl(root->left, dot);
  gen_avl_dot_impl(root->right, dot);
}

// dot 走起来
string_t gen_avl_dot(avl_t *avl) {
  assert(avl);

  // 这个dot和tree的dot没有什么区别呀
  string_t dot = make_string("");
  string_append(&dot, "digraph {\n");
  gen_avl_dot_impl(avl->root, &dot);
  string_append(&dot, "}\n");

  FILE *fp;
  fp = fopen("avl.dot", "w");
  assert(fp);
  fprintf(fp, "%s\n", dot.data);
  fclose(fp);
}

void test_avl() {
  avl_t avl = make_avl();

  for (int i = 0; i < 10; ++i) {
    avl_insert(&avl, i);
    check_avl(&avl);
    gen_avl_dot(&avl);
  }

  for (int i = 0; i < 10; ++i) {
    avl_delete(&avl, i);
    check_avl(&avl);
    gen_avl_dot(&avl);
  }
}

void test_random_avl() {
  // 随机的生成0-99的数字
  // 随机的操作序列 查找 插入 删除
  // 按照baseline进行测试
  set_t set;
  make_set(&set);

  avl_t avl = make_avl();

  rand_t rnum = make_rand(0, 99);
  rand_t rop = make_rand(0, 2);

  int num = 0;
  int op = 0;
  while (true) {
    num = crand(&rnum);
    op = crand(&rop);
    if (op == 0) {
      // search
      assert(set_search(&set, num) == avl_search(&avl, num));
    } else if (op == 1) {
      // insert
      set_insert(&set, num);
      avl_insert(&avl, num);
    } else if (op == 2) {
      // delete
      set_delete(&set, num);
      avl_delete(&avl, num);
    } else {
      assert(false);
    }
    check_avl(&avl);
  }
}

int main(int argc, char *argv[]) {
  test_avl();
  test_random_avl();
  return 0;
}