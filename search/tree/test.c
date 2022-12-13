// 2022/11/10
// zhangzhong
// test binary search tree

#include "tree.h"
#include <assert.h>

void test_tree() {
    tree_t tree;
    tree_init(&tree);

    tree_insert(&tree, 1);
    assert(tree_search(&tree, 1));
    tree_check(&tree);

    assert(tree_max(&tree) == 1);
    assert(tree_min(&tree) == 1);

    tree_insert(&tree, 5);
    tree_insert(&tree, 3);
    tree_insert(&tree, 4);
    tree_insert(&tree, 2);
    assert(tree_max(&tree) == 5);
    assert(tree_min(&tree) == 1);

    tree_free(&tree);
}

int main(int argc, char* argv[]) {}
