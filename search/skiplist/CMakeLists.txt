project(skiplist)

add_library(${PROJECT_NAME} STATIC
    src/skiplist.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC include)

target_link_libraries(${PROJECT_NAME}
    PUBLIC util)
