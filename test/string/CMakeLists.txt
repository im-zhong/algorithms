add_executable(test_string test_string.c)
target_link_libraries(test_string 
    PRIVATE string
    PRIVATE util
    PRIVATE container)

add_executable(compress compress.c)
target_link_libraries(compress 
    PRIVATE string
    PRIVATE util
    PRIVATE container)

add_executable(test_bitbuf test_bitbuf.c)
target_link_libraries(test_bitbuf
    PRIVATE string
    PRIVATE util
    PRIVATE container)
