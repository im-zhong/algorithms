// 2022/11/7
// zhangzhong

#include <container/stack.h>
#include <stdlib.h>

typedef struct __int_stack {
  int i;
  INHERIT_STACK;
} int_stack;

int main(int argc, char *argv[]) {
  stack_init(my_stack);
  int_stack *entry = (int_stack *)malloc(sizeof(int_stack));
  stack_push(&my_stack, entry);

  return 0;
}