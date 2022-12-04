project(util)

add_library(${PROJECT_NAME} STATIC
    src/c_string.c
    src/c_random.c
    src/test.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC include)

