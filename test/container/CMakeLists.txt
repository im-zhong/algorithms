add_executable(test_container test_container.c)

target_link_libraries(test_container 
    PRIVATE container
    PRIVATE util)
