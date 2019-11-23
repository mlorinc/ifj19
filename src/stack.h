#ifndef __H_STACK
#define __H_STACK

#include "deque.h"

typedef struct deque *stack_t;

stack_t stack_init();
int stack_destroy(stack_t stack);
int stack_push(stack_t stack, void *element);
int stack_pop(stack_t stack);
void * stack_top(stack_t stack);
bool stack_empty(stack_t stack);

#endif
