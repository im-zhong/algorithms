// 2023/4/2
// zhangzhong
// test list

#include "container/list.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
    list_node_t node;
    int value;
} entry_t;

entry_t* make_entry(int value) {
    entry_t* entry = malloc(sizeof(entry_t));
    assert(entry != NULL);
    entry->value = value;
    return entry;
}

void free_entry(void* entry) { free(entry); }

void test_simple_list() {
    list_node_t* head = make_list();
    assert(list_is_head(head, head));

    for (int i = 0; i < 10; i++) {
        entry_t* entry = make_entry(i);
        list_insert_after(head, entry);
    }

    entry_t* entry = NULL;
    list_for_each(head, entry) { printf("%d ", entry->value); }

    free_list(head, free_entry);
}

int main() { test_simple_list(); }
