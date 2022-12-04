cmake_minimum_required(VERSION 3.22)

project(Algorithms  VERSION 0.1
                    DESCRIPTION "Algorithms"
                    LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(util)
add_subdirectory(string)
add_subdirectory(test)
