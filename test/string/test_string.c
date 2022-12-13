// 2022/12/4
// zhangzhong
// test string

#include "container/def.h"
#include "container/strset.h"
#include "string/kmp.h"
#include "string/sort.h"
#include "string/suffix_array.h"
#include "string/trie.h"
#include "util/c_random.h"
#include "util/c_string.h"
#include "util/test.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_calculate_prefix() {
    char* pattern = "ababaca";
    size_t prefix[7];
    calculate_prefix(pattern, strlen(pattern), prefix);
}

void test_kmp() {
    // case1
    const char* text = "abababacababaca";
    const char* pattern = "ababaca";
    kmp(pattern, strlen(pattern), text, strlen(text));
}

void test_lsd_sort() {
    // 我们需要一个随机字符串数组
    const size_t n = 16;
    size_t len = 7;
    // 生成随机字符串数组

    char* str[n];
    for (size_t i = 0; i < n; i++) {
        str[i] = random_string(len, len).data;
    }

    lsd_sort(str, n, len);
    check_string_array_order(str, n);

    // free str
    for (size_t i = 0; i < n; i++) {
        free(str[i]);
    }
}

void test_msd_sort() {
    const size_t n = 128;
    size_t minlen = 3;
    size_t maxlen = 15;
    // 生成随机字符串数组
    char* str[n];
    for (size_t i = 0; i < n; i++) {
        str[i] = random_string(minlen, maxlen).data;
    }

    msd_sort(str, n);
    check_string_array_order(str, n);

    // free str
    for (size_t i = 0; i < n; i++) {
        free(str[i]);
    }
}

void test_trie() {
    trie_t trie;
    make_trie(&trie);

    trie_insert(&trie, "hello", 1);
    trie_insert(&trie, "world", 2);

    value_t value = 0;
    assert(trie_search(&trie, "hello", &value));
    assert(value == 1);

    trie_delete(&trie, "hel");
    trie_delete(&trie, "helloo");
    trie_delete(&trie, "hello");
    trie_delete(&trie, "1314");

    free_trie(&trie);
}

// case: insert abc
//        delete abcde
void test_trie_delete_prefix() {
    trie_t trie;
    make_trie(&trie);

    bool r = false;
    value_t v = 0;

    r = trie_search(&trie, "a", NULL);
    assert(r == false);

    trie_insert(&trie, "a", 123);
    r = trie_search(&trie, "a", &v);
    assert(r);
    assert(v == 123);

    // 果然触发了这个bug
    trie_delete(&trie, "ab");
    r = trie_search(&trie, "a", &v);
    assert(r);
    assert(v == 123);

    free_trie(&trie);
}

char** trie_str = NULL;
int pos = 0;
void trie_sort(string_t* str) {
    // 复制给定的字符串并插入到一个数组里面
    int len = strlen(str->data);
    trie_str[pos] = malloc(sizeof(char) * (len + 1));
    strcpy(trie_str[pos], str->data);
    pos++;
}

void test_trie_foreach(trie_t* trie, strset_t* set) {
    int n = set->size;
    strset_entry_t* entry = NULL;

    pos = 0;
    int i = 0;
    int len = 0;
    char** set_str = (char**)malloc(sizeof(char*) * n);
    list_for_each_entry(entry, &set->head, strset_entry_t, node) {
        len = strlen(entry->str);
        set_str[i] = malloc(sizeof(char) * (len + 1));
        strcpy(set_str[i], entry->str);
        i++;
    }

    trie_str = (char**)malloc(sizeof(char*) * n);
    trie_foreach(trie, trie_sort);

    // 对两个数组进行排序
    msd_sort(set_str, n);
    msd_sort(trie_str, n);
    // 首先检查顺序
    check_string_array_order(set_str, n);
    check_string_array_order(trie_str, n);
    // 然后检查两个数组是否完全一致
    for (int i = 0; i < n; i++) {
        assert(strcmp(set_str[i], trie_str[i]) == 0);
    }
    // 释放他们的内存
    for (int i = 0; i < n; i++) {
        // free(set_str[i]);
        free(trie_str[i]);
    }
    // free(set_str);
    free(trie_str);
    // set_str = NULL;
    trie_str = NULL;
    pos = 0;
}

// 再写一个辅助测试函数 任何一次操作之后
// 两个集合的数据应该一样
void test_random_trie_helper(trie_t* trie, strset_t* set) {
    // 遍历set 对于其中的每个string进行查找
    // 两个数据集应该一样
    assert(trie);
    assert(set);
    bool r = false;
    value_t v = 0;

    int n = 0;
    strset_entry_t* entry = NULL;
    list_for_each_entry(entry, &set->head, strset_entry_t, node) {
        r = strset_search(set, entry->str, &v);
        assert(r);
        assert(v == entry->value);

        r = trie_search(trie, entry->str, &v);
        if (!r) {
            printf("%s\n", entry->str);
            fflush(stdout);
        }
        assert(r);
        assert(v == entry->value);

        n++;
    }
    assert(n == set->size);
}

// random test 非常困难呀 我们需要一个 set<string>
// 但是C并没有这样的东西
// 这是否有一点离谱 还得实现一个string set
void test_random_trie() {
    trie_t trie;
    make_trie(&trie);
    strset_t set;
    make_strset(&set);

    size_t minlen = 1;
    size_t maxlen = 7;

    value_t v1 = 0;
    value_t v2 = 0;
    bool r1 = false;
    bool r2 = false;
    int num = 0;
    int op = 0;
    while (true) {
        string_t str = random_string(minlen, maxlen);
        op = rrand(0, 2);
        num = rrand(0, 1000000);
        v1 = 0;
        v2 = 0;
        r1 = false;
        r2 = false;
        printf("op: %d, key: %s, value: %d\n", op, str.data, num);

        if (op == 0) {
            // search
            r1 = strset_search(&set, str.data, &v1);
            r2 = trie_search(&trie, str.data, &v2);
            if (r1 != r2) {
                printf("strset: %s, trie: %s\n", r1 ? "true" : "false",
                       r2 ? "true" : "false");
                assert(false);
            }
            if (v1 != v2) {
                printf("v1: %ld, v2: %ld\n", v1, v2);
                assert(false);
            }
        } else if (op == 1) {
            // insert
            strset_insert(&set, str.data, num);
            trie_insert(&trie, str.data, num);
        } else if (op == 2) {
            // delete
            strset_delete(&set, str.data);
            trie_delete(&trie, str.data);
        } else {
            assert(false);
        }
        free_string(&str);

        test_random_trie_helper(&trie, &set);
        test_trie_foreach(&trie, &set);
    }

    free_trie(&trie);
    free_strset(&set);
}

void test_lrs() {
    // 生成随机字符串
    while (true) {
        string_t str = random_string(256, 1024);

        string_t lrs1 = lrs_slow(str.data);
        string_t lrs2 = lrs(str.data);
        assert(strcmp(lrs1.data, lrs2.data));

        free_string(&str);
    }
}

void test_kwic(int argc, char** argv) {
    FILE* stream;
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;

    if (argc == 1) {
        stream = stdin;
    } else if (argc == 2) {
        stream = fopen(argv[1], "r");
        if (stream == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    string_t str = make_string("");
    while ((nread = getline(&line, &len, stream)) != -1) {
        string_concat(&str, line, nread);
    }

    free(line);
    fclose(stream);

    // 然后开始执行kwic
    suffix_array_t* sa = make_suffix_array(str.data, str.size);
    // 然后等待用户输入字符串
    char input[1024] = {'\0'};
    while (true) {
        printf("> ");
        scanf("%1023s", input);
        kwic(sa, input);
    }
}

int main(int argc, char* argv[]) {
    // test_calculate_prefix();
    // test_kmp();
    // test_lsd_sort();
    // test_msd_sort();
    // test_trie();
    // test_trie_delete_prefix();
    // // test_random_trie();
    // test_lrs();

    test_kwic(argc, argv);

    return 0;
}
