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
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <string>

std::string btree_node_to_string(btree_node_t* node) {
    std::string name;
    for (size_t i = 0; i < node->size; ++i) {
        name += std::to_string(node->keys[i]);
        if (i != node->size - 1) {
            name += " ";
        }
    }
    return name;
}

void gen_btree_dot_impl(btree_node_t* root, std::string& dot) {
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

void gen_btree_dot(btree_t* btree) {
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

void btree_check_impl(btree_node_t* root) {
    if (!root) {
        return;
    }
    // 1. 我们的key是有顺序的
    // 2. 我们孩子的key的范围是在key的范围之间的
    // 3. 递归就行了
    array_check(root->keys, root->size);

    for (size_t i = 0; i < root->size + 1; ++i) {
        btree_node_t* child = root->childs[i];
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
void btree_check(btree_t* btree) {
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

// 右旋没写对
void test_random_btree_bug1() {
    btree_t btree;
    btree_init(&btree);

    btree_search(&btree, 59);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 86);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 91);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 28);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 47);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 34);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 52);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 63);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 54);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 71);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 11);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 38);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 73);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 22);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 29);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 20);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 91);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 28);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 68);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 18);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 59);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 64);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 91);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 96);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 75);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 39);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 88);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 37);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 44);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 24);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 58);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 83);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 45);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 61);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 68);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 94);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 6);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 66);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 1);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 31);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 40);
    btree_check(&btree);
    gen_btree_dot(&btree);

    btree_free(&btree);
}

void test_random_btree_bug2() {
    btree_t btree;
    btree_init(&btree);

    btree_search(&btree, 84);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 23);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 49);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 24);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 7);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 53);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 94);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 8);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 16);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 56);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 79);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 78);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 96);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 89);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 7);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 13);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 40);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 69);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 3);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 40);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_search(&btree, 17);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_insert(&btree, 12);
    btree_check(&btree);
    gen_btree_dot(&btree);
    btree_delete(&btree, 67);
    btree_check(&btree);
    gen_btree_dot(&btree);

    btree_free(&btree);
}

void test_random_btree() {
    // 我们会随机生成 btree
    // 不断增加数据并且每增加一次数据都会执行 btree_check
    // 随机的增加数据 随机的删除数据 随机的search数据
    // 我们会使用一个 std::set 作为测试基准 行为要求一致时 才会通过测试
    std::default_random_engine e(
        std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> ui(0, 100);
    // 0 表示 search
    // 1 insert
    // 2 delete
    std::uniform_int_distribution<int> uop(0, 2);

    std::set<value_t> baseline_set;

    btree_t btree;
    btree_init(&btree);

    // 每次动作都写成函数代码
    // 如果出错了 直接复制输出 可以作为新的测试用例 复现这个bug
    // 不愧是我
    // 输出是可以控制的 可以设置一个debug标志
    for (size_t i = 0; i < 100; ++i) {
        int key = ui(e);
        int op = uop(e);

        if (op == 0) {
            printf("btree_search(&btree, %d);\n", key);
            printf("btree_check(&btree);\n");
            printf("gen_btree_dot(&btree);\n");
            bool baseline_r = baseline_set.find(key) != baseline_set.end();
            bool r = btree_search(&btree, key);
            btree_check(&btree);
            // 每次操作成功之后都生成dot
            // 这样如果测试失败了
            // 使用dot命令就可以看到最后一次测试成功的b树的样子 再结合输出
            // 就知道是什么操作导致的bug了
            gen_btree_dot(&btree);
            assert(baseline_r == r);
        } else if (op == 1) {
            printf("btree_insert(&btree, %d);\n", key);
            printf("btree_check(&btree);\n");
            printf("gen_btree_dot(&btree);\n");
            baseline_set.insert(key);
            btree_insert(&btree, key);
            btree_check(&btree);
            gen_btree_dot(&btree);
            assert(btree_search(&btree, key));
        } else if (op == 2) {
            printf("btree_delete(&btree, %d);\n", key);
            printf("btree_check(&btree);\n");
            printf("gen_btree_dot(&btree);\n");
            baseline_set.erase(key);
            btree_delete(&btree, key);
            btree_check(&btree);
            gen_btree_dot(&btree);
            assert(!btree_search(&btree, key));
        } else {
            assert(false);
        }
    }

    btree_free(&btree);
}

int main(int argc, char* argv[]) {
    test_btree();
    test_btree_delete();
    test_random_btree_bug1();
    test_random_btree_bug2();
    for (int i = 0; i < 100000; ++i) {
        printf("\n\n");
        test_random_btree();
    }
}