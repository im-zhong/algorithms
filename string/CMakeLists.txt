project(string)

add_library(${PROJECT_NAME} STATIC
    src/kmp.c
    src/sort.c
    src/trie.c
    src/suffix_array.c
    src/huffman.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC include)

target_link_libraries(${PROJECT_NAME}
    PUBLIC util
    PUBLIC container)
