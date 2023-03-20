project(new_compress)

add_library(new_compress STATIC 
    src/run_length_encoding.c
    src/lzw.c
)
target_include_directories(new_compress 
    PUBLIC include
)
target_link_libraries(new_compress 
    PUBLIC container
    PUBLIC util
)

#1 test run length encoding
add_executable(test_run_length_encoding 
    test/test_run_length_encoding.c
)
target_link_libraries(test_run_length_encoding 
    PUBLIC new_compress
)

#2 test lzw
add_executable(test_lzw 
    test/test_lzw.c
)
target_link_libraries(test_lzw 
    PUBLIC new_compress
)
