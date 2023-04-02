add_library(listv2 STATIC
    src/list.c)
target_include_directories(listv2 PUBLIC include)

add_executable(test_listv2 test/test_list.c)
target_link_libraries(test_listv2 listv2)
