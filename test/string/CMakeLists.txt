add_executable(test_string test_string.c)
target_link_libraries(test_string 
    PRIVATE string
    PRIVATE util
    PRIVATE container)
