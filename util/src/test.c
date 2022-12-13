// 2022/12/4
// zhangzhong
// a simple test framework

#include "util/test.h"
#include "util/c_random.h"
#include <assert.h>
#include <string.h>

void check_array_order(value_t* data, size_t size) {
    assert(data);
    if (size > 0) {
        for (size_t i = 0; i < size - 1; ++i) {
            assert(data[i] <= data[i + 1]);
        }
    }
}

// 还要给一个rand_t类型是真的扯淡 难道就不能静态吗 ？？？
string_t random_string(size_t min, size_t max) {
    int len = rrand(min, max);
    string_t str = make_string("");
    for (size_t i = 0; i < len; i++) {
        string_append(&str, "%c", (char)rrand('a', 'g'));
    }
    return str;
}

void check_string_array_order(char** str, size_t n) {
    assert(str);
    for (size_t i = 1; i < n; i++) {
        assert(str[i - 1]);
        assert(str[i]);
        assert(strcmp(str[i - 1], str[i]) <= 0);
    }
}
