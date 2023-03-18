project(expr)

# expr static library
add_library(expr STATIC
    src/infix_expr.c
    src/suffix_expr.c
)
target_include_directories(expr 
    PUBLIC include
)
target_link_libraries(expr 
    PUBLIC util
    PUBLIC container
)

#1 test suffix expression evaluation
add_executable(test_eval_suffix_expr
    test/test_eval_suffix_expr.c
)
target_link_libraries(test_eval_suffix_expr
    PUBLIC expr
    PUBLIC container
)
