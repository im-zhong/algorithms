// 2022/11/6
// zhangzhong
// test_array.cpp

#include "array.h"
#include <assert.h>
#include <stdio.h>

void test_array_search() {
    value_t array[7] = {1, 2, 3, 4, 5, 6, 7};
    array_check(array, 7);

    // want: r = true, pos = 0
    size_t pos = 0;
    bool r = array_search(array, 7, 1, &pos);
    if (!(r == true && pos == 0)) {
        printf(
            "%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
            __FILE__, __LINE__, __func__, true, 0lu, r, pos);
    }

    // want: r = true, pos = 6
    pos = 0;
    r = array_search(array, 7, 7, &pos);
    if (!(r == true && pos == 6)) {
        printf(
            "%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
            __FILE__, __LINE__, __func__, true, 6lu, r, pos);
    }

    // want: r = false, pos = 0
    pos = 0;
    r = array_search(array, 7, -1, &pos);
    if (!(r == false && pos == 0)) {
        printf(
            "%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
            __FILE__, __LINE__, __func__, false, 0lu, r, pos);
    }

    // want: r = false, pos = 7
    pos = 0;
    r = array_search(array, 7, 8, &pos);
    if (!(r == false && pos == 7)) {
        printf(
            "%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
            __FILE__, __LINE__, __func__, false, 7lu, r, pos);
    }
}

bool array_equal(value_t* array1, size_t narray1, value_t* array2,
                 size_t narray2) {
    if (narray1 != narray2) {
        return false;
    }
    for (size_t i = 0; i < narray1; ++i) {
        if (array1[i] != array2[i]) {
            return false;
        }
    }
    return true;
}

void test_array_delete() {
    // want r = true
    value_t array[8] = {1, 2, 3, 4, 5, 6, 7};
    value_t array1[6] = {2, 3, 4, 5, 6, 7};
    // delete 1
    array_delete(array, 7, 0);
    bool r = array_equal(array, 6, array1, 6);
    if (!(r == true)) {
        printf("%s:%d %s failed. want r = %d, but r = %d.\n", __FILE__,
               __LINE__, __func__, true, r);
    }

    // want r = true
    // array = {2, 3, 4, 5, 6, 7}
    value_t array2[5] = {2, 3, 4, 5, 6};
    // delete 7
    array_delete(array, 6, 5);
    r = array_equal(array, 5, array2, 5);
    if (!(r == true)) {
        printf("%s:%d %s failed. want r = %d, but r = %d.\n", __FILE__,
               __LINE__, __func__, true, r);
    }

    // want r = true
    // array = {2, 3, 4, 5, 6}
    value_t array3[5] = {2, 3, 5, 6};
    // delete 4
    array_delete(array, 5, 2);
    r = array_equal(array, 4, array3, 4);
    if (!(r == true)) {
        printf("%s:%d %s failed. want r = %d, but r = %d.\n", __FILE__,
               __LINE__, __func__, true, r);
    }
}

void test_array_insert() {
    // case 1: insert 1
    value_t array[8] = {2, 3, 5, 6};
    value_t array1[] = {1, 2, 3, 5, 6};
    size_t pos = array_insert(array, 4, 1);
    bool r = array_equal(array, 5, array1, 5);
    if (!(r == true && pos == 0)) {
        printf(
            "%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
            __FILE__, __LINE__, __func__, true, 0lu, r, pos);
    }

    // case 2: insert 4
    // array[] = {1, 2, 3, 5, 6}
    value_t array2[] = {1, 2, 3, 4, 5, 6};
    pos = array_insert(array, 5, 4);
    r = array_equal(array, 6, array2, 6);
    if (!(r == true && pos == 3)) {
        printf(
            "%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
            __FILE__, __LINE__, __func__, true, 3lu, r, pos);
    }

    // case 3: insert 7
    // array[] = {1, 2, 3, 4, 5, 6}
    value_t array3[] = {1, 2, 3, 4, 5, 6, 7};
    pos = array_insert(array, 6, 7);
    r = array_equal(array, 7, array3, 7);
    if (!(r == true && pos == 6)) {
        printf(
            "%s:%d %s failed. want r = %d, pos = %lu, but r = %d, pos = %lu.\n",
            __FILE__, __LINE__, __func__, true, 6lu, r, pos);
    }
}

void test_array_insert_array() {
    value_t array[16] = {3, 4, 6, 7};
    size_t array_size = 4;
    value_t array1[] = {1, 2};
    size_t array1_size = sizeof(array1) / sizeof(value_t);
    value_t array2[] = {5};
    size_t array2_size = sizeof(array2) / sizeof(value_t);
    value_t array3[] = {8, 9, 10};
    size_t array3_size = sizeof(array3) / sizeof(value_t);

    // case 1: insert at front
    array_insert_array(array, array_size, array1, array1_size, 0);
    array_size += array1_size;
    value_t tmp1[] = {1, 2, 3, 4, 6, 7};
    size_t tmp1_size = sizeof(tmp1) / sizeof(value_t);
    bool r = array_equal(array, array_size, tmp1, tmp1_size);
    if (!(r == true)) {
        printf("%s:%d %s failed. want r = %d, but r = %d.\n", __FILE__,
               __LINE__, __func__, true, r);
    }

    // case 2: insert at middle
    array_insert_array(array, array_size, array2, array2_size, 4);
    array_size += array2_size;
    value_t tmp2[] = {1, 2, 3, 4, 5, 6, 7};
    size_t tmp2_size = sizeof(tmp2) / sizeof(value_t);
    r = array_equal(array, array_size, tmp2, tmp2_size);
    if (!(r == true)) {
        printf("%s:%d %s failed. want r = %d, but r = %d.\n", __FILE__,
               __LINE__, __func__, true, r);
    }

    // case 3: insert at back
    array_insert_array(array, array_size, array3, array3_size, array_size);
    array_size += array3_size;
    value_t tmp3[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t tmp3_size = sizeof(tmp3) / sizeof(value_t);
    r = array_equal(array, array_size, tmp3, tmp3_size);
    if (!(r == true)) {
        printf("%s:%d %s failed. want r = %d, but r = %d.\n", __FILE__,
               __LINE__, __func__, true, r);
    }
}

int main(int argc, char* argv[]) {
    test_array_search();
    test_array_delete();
    test_array_insert();
    test_array_insert_array();
}
