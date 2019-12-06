#ifndef __H_QUEUE
#define __H_QUEUE

#include "deque.h"

typedef struct deque *queue_t;

queue_t queue_init();
size_t queue_size(queue_t queue);
bool queue_empty(queue_t queue);
bool queue_destroy(queue_t queue);
bool queue_push(queue_t queue, void *element);
void * queue_pop(queue_t queue);
void * queue_front(queue_t queue);
iterator_t queue_begin(queue_t queue);
iterator_t queue_end(queue_t queue);

#endif
