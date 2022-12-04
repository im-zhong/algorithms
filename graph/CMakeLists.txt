project(graph)

add_library(${PROJECT_NAME} STATIC
    src/spare_graph.c
    src/shortest_path.c
    src/minimum_spanning_tree.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC include)

target_link_libraries(${PROJECT_NAME}
    PUBLIC container)
