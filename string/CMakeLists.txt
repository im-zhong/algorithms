project(string)

add_library(${PROJECT_NAME} STATIC
    src/kmp.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC include)
