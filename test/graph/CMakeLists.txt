add_executable(test_graph test_graph.c)

target_link_libraries(test_graph
    PRIVATE graph
    PRIVATE util)
