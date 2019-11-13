#include "queue.h"

queue_t queue_init() {
    return deque_init();
}

int queue_destroy(queue_t queue) {
    return deque_destroy(queue);
}

int queue_push(queue_t queue, void *element) {
    return deque_push_back(queue, element);
}

int queue_pop(queue_t queue) {
    return deque_pop_front(queue);
}

void *queue_front(queue_t queue) {
    return deque_front(queue);
}
