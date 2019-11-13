#ifndef __H_QUEUE
#define __H_QUEUE

#include "deque.h"

typedef struct deque *queue_t;

queue_t queue_init();
int queue_destroy(queue_t queue);
int queue_push(queue_t queue, void *element);
int queue_pop(queue_t queue);
void * queue_front(queue_t queue);

#endif
