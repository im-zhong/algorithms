// 2023/3/20
// zhangzhong

#include "compress/lzw.h"
#include "util/file.h"
#include <assert.h>
#include <stdbool.h>

void test_lzw() {
    const char* file = "lzw.txt";
    const char* compressed_file = "lzw.txt.compress";
    const char* decompressed_file = "lzw.txt.decompress";
    assert(generate_random_file(file, 4096) == 0);

    assert(lzw_compression(file, compressed_file) == 0);
    assert(lzw_expansion(compressed_file, decompressed_file) == 0);
}

int main(int argc, char* argv[]) {
    test_lzw();
    return 0;
}
