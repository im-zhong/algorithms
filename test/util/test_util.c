#include "util/c_string.h"

void test_string() {
    // 这里传数就不对哎 为什么
    string_t src = make_string("hello %d\n", 12345);
    for (int i = 0; i < 10; ++i) {
        string_append(&src, "%d run\n", i);
    }
    string_append(&src, "\n");
    printf("%s\n", src.data);
    string_free(&src);
}

int main(int argc, char* argv[]) { test_string(); }
