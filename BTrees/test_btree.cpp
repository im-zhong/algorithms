// 2022/11/5
// zhangzhong
// 用C写实在是太麻烦了
// 还是用C++吧
// 虽然用C++写 但是这些数据结构也不会写成模板
// 因为我们的重点并不在于模板 加入模板会让调试变得非常困难
// 而且并不清晰
// 使用C++的目的只在于C++提供了一些非常方便的库函数

#include "array.h"
#include "btree.h"
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

std::string btree_node_to_string(btree_node_t *node) {
  std::string name;
  for (size_t i = 0; i < node->size; ++i) {
    name += std::to_string(node->keys[i]);
    if (i != node->size - 1) {
      name += " ";
    }
  }
  return name;
}

void gen_btree_dot_impl(btree_node_t *root, std::string &dot) {
  if (!root) {
    return;
  }

  // 1. 遍历所有的key 生成一个名字
  // 2. 遍历所有的孩子的key 每个孩子生成一个名字
  // 然后对于每个这样的链接 生成一行
  std::string parent_name = btree_node_to_string(root);
  // 对于每一个节点 根据是否是叶子节点 形状也不同
  if (root->is_leaf) {
    dot += ("  \"" + parent_name + "\" [shape=box]\n");
  } else {
    dot += ("  \"" + parent_name + "\" [shape=circle]\n");
  }
  for (size_t i = 0; i < root->size + 1; ++i) {
    // child并不一定存在 可能是空指针
    if (root->childs[i]) {
      std::string child_name = btree_node_to_string(root->childs[i]);
      // 生成一行dot
      dot += ("  \"" + parent_name + "\" -> \"" + child_name + "\"\n");
    }
  }
  if (!root->is_leaf) {
    for (size_t i = 0; i < root->size + 1; ++i) {
      gen_btree_dot_impl(root->childs[i], dot);
    }
  }
}

void gen_btree_dot(btree_t *btree) {
  // 遍历b树
  if (!btree) {
    return;
  }
  if (!btree->root) {
    return;
  }

  // 遍历是递归的，所以需要写一个辅助函数
  std::string dot;
  dot += "digraph {\n";
  gen_btree_dot_impl(btree->root, dot);
  dot += "}\n";

  std::ofstream fout;
  fout.open("btree.dot");
  fout << dot;
  fout.close();
}

void btree_check_impl(btree_node_t *root) {
  if (!root) {
    return;
  }
  // 1. 我们的key是有顺序的
  // 2. 我们孩子的key的范围是在key的范围之间的
  // 3. 递归就行了
  array_check(root->keys, root->size);

  for (size_t i = 0; i < root->size + 1; ++i) {
    btree_node_t *child = root->childs[i];
    if (child) {
      // 也要遍历child的所有的key
      // 并且与我们自己的key进行比较
      if (i == 0) {
        // 最开头的一个child
        for (size_t j = 0; j < child->size; ++j) {
          assert(child->keys[j] < root->keys[i]);
        }
      } else if (i == root->size) {
        // 最后一个child
        for (size_t j = 0; j < child->size; ++j) {
          assert(child->keys[j] > root->keys[i - 1]);
        }
      } else {
        // 中间的child 需要和两侧的值比较
        for (size_t j = 0; j < child->size; ++j) {
          assert(child->keys[j] > root->keys[i - 1]);
          assert(child->keys[j] < root->keys[i]);
        }
      }
    }
  }

  // 检查完这一个节点之后， 遍历所有的孩子节点 并进行检查
  for (size_t i = 0; i < root->size + 1; ++i) {
    btree_check_impl(root->childs[i]);
  }
}

// 写一个程序 验证是否为b树
void btree_check(btree_t *btree) {
  if (!btree) {
    return;
  }
  if (!btree->root) {
    return;
  }
  btree_check_impl(btree->root);
}

// 先测试btree
void test_btree() {
  btree_t btree;
  btree_init(&btree);
  btree_insert(&btree, 1);
  btree_check(&btree);
  btree_insert(&btree, 2);
  btree_check(&btree);
  btree_insert(&btree, 3);
  btree_check(&btree);
  btree_insert(&btree, 4);
  btree_check(&btree);
  btree_insert(&btree, 5);
  btree_check(&btree);
  btree_insert(&btree, 6);
  btree_check(&btree);
  btree_insert(&btree, 7);
  btree_check(&btree);
  btree_insert(&btree, 8);
  btree_check(&btree);
  btree_insert(&btree, 9);
  btree_check(&btree);
  btree_insert(&btree, 10);
  btree_check(&btree);
  btree_insert(&btree, 11);
  btree_check(&btree);
  btree_insert(&btree, 12);
  btree_check(&btree);
  btree_insert(&btree, 13);
  gen_btree_dot(&btree);

  assert(btree_search(&btree, 5));
  assert(!btree_search(&btree, 0));
  assert(!btree_search(&btree, 15));

  btree_free(&btree);
}

void test_btree_delete() {
  btree_t btree;
  btree_init(&btree);
  btree_insert(&btree, 1);
  btree_check(&btree);
  btree_insert(&btree, 2);
  btree_check(&btree);
  btree_insert(&btree, 3);
  btree_check(&btree);
  btree_insert(&btree, 4);
  btree_check(&btree);
  btree_insert(&btree, 5);
  btree_check(&btree);
  btree_insert(&btree, 6);
  btree_check(&btree);
  btree_insert(&btree, 7);
  btree_check(&btree);
  btree_insert(&btree, 8);
  btree_check(&btree);
  btree_insert(&btree, 9);
  btree_check(&btree);
  btree_insert(&btree, 10);
  btree_check(&btree);
  btree_insert(&btree, 11);
  btree_check(&btree);
  btree_insert(&btree, 12);
  btree_check(&btree);
  btree_insert(&btree, 13);
  btree_check(&btree);
  gen_btree_dot(&btree);

  btree_delete(&btree, 5);
  btree_check(&btree);
  gen_btree_dot(&btree);

  btree_delete(&btree, 4);
  btree_check(&btree);
  gen_btree_dot(&btree);

  btree_delete(&btree, 1);
  btree_check(&btree);
  gen_btree_dot(&btree);
  btree_delete(&btree, 2);
  btree_check(&btree);
  gen_btree_dot(&btree);
  btree_delete(&btree, 3);
  btree_check(&btree);
  gen_btree_dot(&btree);
  btree_delete(&btree, 4);
  btree_check(&btree);
  btree_delete(&btree, 5);
  btree_check(&btree);
  gen_btree_dot(&btree);
  btree_delete(&btree, 6);
  btree_check(&btree);
  gen_btree_dot(&btree);
  btree_delete(&btree, 7);
  btree_check(&btree);
  gen_btree_dot(&btree);
  btree_delete(&btree, 8);
  btree_check(&btree);
  gen_btree_dot(&btree);
  btree_delete(&btree, 9);
  btree_check(&btree);
  gen_btree_dot(&btree);
  btree_delete(&btree, 10);
  btree_check(&btree);
  gen_btree_dot(&btree);
  btree_delete(&btree, 11);
  btree_check(&btree);
  gen_btree_dot(&btree);
  btree_delete(&btree, 12);
  btree_check(&btree);
  gen_btree_dot(&btree);
  btree_delete(&btree, 13);
  btree_check(&btree);
  gen_btree_dot(&btree);

  assert(!btree_search(&btree, 13));

  btree_free(&btree);
}

void test_random_btree() {
  // 我们会随机生成 btree
  // 不断增加数据并且每增加一次数据都会执行 btree_check
  std::default_random_engine e(
      std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> ui(0, 100);

  btree_t btree;
  btree_init(&btree);

  for (size_t i = 0; i < 50; ++i) {
    int key = ui(e);
    btree_insert(&btree, key);
    btree_check(&btree);
    assert(btree_search(&btree, key));
  }

  btree_free(&btree);
}

int main(int argc, char *argv[]) {
  // test_btree();
  // for (int i = 0; i < 1000000; ++i) {
  //   test_random_btree();
  // }
  test_btree_delete();
}