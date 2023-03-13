project(tabular)

add_library(tabular STATIC
    src/rod_cutting.c)
target_include_directories(tabular PUBLIC include)
