#include "queue.h"

queue_t queue_init() {
    return deque_init();
}

size_t queue_size(queue_t queue) {
    return deque_size(queue);
}

bool queue_empty(queue_t queue) {
    return queue_size(queue) == 0;
}

bool queue_destroy(queue_t queue) {
    return deque_destroy(queue);
}

bool queue_push(queue_t queue, void *element) {
    return deque_push_back(queue, element);
}

void *queue_pop(queue_t queue) {
    return deque_pop_front(queue);
}

void *queue_front(queue_t queue) {
    return deque_front(queue);
}

iterator_t queue_begin(queue_t queue) {
    return deque_begin(queue);
}

iterator_t queue_end(queue_t queue) {
    return deque_end(queue);
}
