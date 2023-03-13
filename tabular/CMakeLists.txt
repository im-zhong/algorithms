project(tabular)

add_library(tabular STATIC
    src/rod_cutting.c
    src/matrix_chain_multiplication.c
    src/longest_common_subsequence.c)
target_include_directories(tabular PUBLIC include)
