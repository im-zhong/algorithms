add_executable(test_sort test_sort.c)
target_link_libraries(test_sort
    PRIVATE sort
    PRIVATE util)

add_executable(test_external_sort test_external_sort.c)
target_link_libraries(test_external_sort
    PRIVATE sort
    PRIVATE util)
