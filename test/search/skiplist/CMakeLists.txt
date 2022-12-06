add_executable(test_skiplist test_skiplist.c)

target_link_libraries(test_skiplist
    PRIVATE util
    PRIVATE container
    PRIVATE skiplist)
