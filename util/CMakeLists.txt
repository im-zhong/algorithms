project(util)

add_library(${PROJECT_NAME} STATIC
    src/c_string.c
    src/c_random.c
    src/test.c
    src/bitbuf.c
)
target_include_directories(${PROJECT_NAME}
    PUBLIC include)
target_link_libraries(${PROJECT_NAME}
    PUBLIC container
)
