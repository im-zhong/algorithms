// 2022/12/6
// zhangzhong
// test skiplist

#include "container/set.h"
#include "skiplist/skiplist.h"
#include "util/c_string.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

string_t skipnode_name(int level, value_t value) {
    if (value == INT64_MIN) {
        return make_string("\"%d/%s\"", level, "min");
    }
    if (value == INT64_MAX) {
        return make_string("\"%d/%s\"", level, "max");
    }
    return make_string("\"%d/%ld\"", level, value);
}

string_t gen_skiplist_dot(skiplist_t* sl) {
    assert(sl);
    // 先写一下怎么生成dot文件
    // 那我们就按层序遍历
    string_t dot = make_string("digraph {\n");
    skipnode_t* header = sl->header;
    int level = sl->level;
    while (header != &sl->bottom) {
        // 遍历整个链表
        // 每个节点的名字就是 "level/value"
        for (skipnode_t* work = header; work != &sl->tail; work = work->right) {
            // [level/work] -> [level/work->right]
            string_t work_name = skipnode_name(level, work->value);
            string_t right_name = skipnode_name(level, work->right->value);
            string_append(&dot, "  %s -> %s\n", work_name.data,
                          right_name.data);
            free_string(&work_name);
            free_string(&right_name);
            // [level/work] -> [level-1/work->down]
            if (work->down != &sl->bottom) {
                string_t work_name = skipnode_name(level, work->value);
                string_t down_name =
                    skipnode_name(level - 1, work->down->value);
                string_append(&dot, "  %s -> %s\n", work_name.data,
                              down_name.data);
                free_string(&work_name);
                free_string(&down_name);
            }
        }
        // 遍历完这一层，前往下一层
        header = header->down;
        level--;
    }
    string_append(&dot, "}\n");
    FILE* fp = fopen("skiplist.dot", "w");
    assert(fp);
    fprintf(fp, "%s\n", dot.data);
    fclose(fp);
    free_string(&dot);
    return dot;
}

void test_skiplist() {
    skiplist_t sl;
    make_skiplist(&sl);

    // 测试一下insert

    skiplist_insert(&sl, 1);
    skiplist_insert(&sl, 2);
    skiplist_insert(&sl, 3);
    skiplist_insert(&sl, 4);
    skiplist_insert(&sl, 5);
    skiplist_insert(&sl, 6);
    skiplist_insert(&sl, 7);
    check_skiplist(&sl);
    gen_skiplist_dot(&sl);

    skiplist_delete(&sl, 1);
    skiplist_delete(&sl, 2);
    skiplist_delete(&sl, 3);
    skiplist_delete(&sl, 4);
    skiplist_delete(&sl, 5);
    skiplist_delete(&sl, 6);
    skiplist_delete(&sl, 7);
    check_skiplist(&sl);
    gen_skiplist_dot(&sl);

    free_skiplist(&sl);
}

void test_insert_duplicate() {
    skiplist_t sl;
    make_skiplist(&sl);

    // 测试一下insert

    skiplist_insert(&sl, 1);
    skiplist_insert(&sl, 1);
    skiplist_insert(&sl, 1);
    gen_skiplist_dot(&sl);
    check_skiplist(&sl);

    free_skiplist(&sl);
}

void test_random_skiplist() {

    // 和 set 做 基准测试
    // 随机操作序列
    // 随机的生成0-99的数字
    // 随机的操作序列 查找 插入 删除
    // 按照baseline进行测试
    set_t set;
    make_set(&set);

    skiplist_t sl;
    make_skiplist(&sl);

    rand_t rnum = make_rand(0, 99);
    rand_t rop = make_rand(0, 2);

    int num = 0;
    int op = 0;
    while (true) {
        num = crand(&rnum);
        op = crand(&rop);
        if (op == 0) {
            // search
            assert(set_search(&set, num) == skiplist_search(&sl, num));
        } else if (op == 1) {
            // insert
            set_insert(&set, num);
            skiplist_insert(&sl, num);
        } else {
            // delete
            set_delete(&set, num);
            skiplist_delete(&sl, num);
        }
        // gen_skiplist_dot(&sl);
        // printf("op: %d, num: %d\n", op, num);
        check_skiplist(&sl);
    }
}

int main() {
    test_skiplist();
    test_insert_duplicate();
    test_random_skiplist();
    return 0;
}