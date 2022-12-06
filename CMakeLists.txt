cmake_minimum_required(VERSION 3.22)

project(Algorithms  VERSION 0.1
                    DESCRIPTION "Algorithms"
                    LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(util)
add_subdirectory(string)
add_subdirectory(container)
add_subdirectory(sort)
add_subdirectory(graph)
# add_subdirectory(search)
add_subdirectory(test)
