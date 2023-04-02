// 2023/3/19
// zhanzhong
// test run length encoding

#include "compress/run_length_encoding.h"
#include "util/file.h"
#include <assert.h>
#include <stdbool.h>

void test_random_run_length_encoding() {
    const char* file = "run_length_encoding.txt";
    const char* compressed_file = "run_length_encoding.txt.compress";
    const char* decompressed_file = "run_length_encoding.txt.decompress";
    assert(generate_random_file(file, 4096) == 0);

    assert(run_length_compress(file, compressed_file) == 0);
    assert(run_length_decompress(compressed_file, decompressed_file) == 0);
}

int main() {
    test_run_length_encoding();
    test_random_run_length_encoding();
    return 0;
}
