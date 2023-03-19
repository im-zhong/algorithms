add_executable(test_util 
    test_util.c
)
target_link_libraries(test_util PRIVATE util)


add_executable(test_bitbuf test_bitbuf.c)
target_link_libraries(test_bitbuf
    PRIVATE util
    PRIVATE container)
