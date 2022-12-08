project(string)

add_library(${PROJECT_NAME} STATIC
    src/kmp.c
    src/sort.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC include)

target_link_libraries(${PROJECT_NAME}
    PUBLIC util)
