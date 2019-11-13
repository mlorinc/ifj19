#include "stack.h"

stack_t stack_init() {
    return deque_init();
}

int stack_destroy(stack_t stack) {
    return deque_destroy(stack);
}

int stack_push(stack_t stack, void *element) {
    return deque_push_back(stack, element);
}

int stack_pop(stack_t stack) {
    return deque_pop_back(stack);
}

void *stack_top(stack_t stack) {
    return deque_back(stack);
}
