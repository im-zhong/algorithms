// 2023/4/2
// zhangzhong
// test vector

#include "container/vector.h"
#include <assert.h>

void test_simple() {
    vector_t* vector = make_vector(0, sizeof(int));
    assert(vector->size == 0);
    assert(vector->capacity == 0);
    assert(vector->type_size == sizeof(int));
    assert(vector->data == NULL);

    // int i = 123;
    // vector_push_back(vector, &i);
    // int* pi = vector_at(vector, 0);
    // assert(*pi == i);

    for (int i = 0; i < 10; i++) {
        vector_push_back(vector, &i);
    }
    assert(vector_size(vector) == 10);

    for (size_t i = 0; i < vector_size(vector); i++) {
        int* value = vector_at(vector, i);
        assert(*value == (int)i);
    }

    free_vector(vector);
}

int main() { test_simple(); }
