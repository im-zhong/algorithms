project(sort)

add_library(${PROJECT_NAME} STATIC
    src/sort.c
    src/external_sort.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC include)

target_link_libraries(${PROJECT_NAME}
    PUBLIC container)
