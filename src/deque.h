#ifndef __H_DEQUE
#define __H_DEQUE

#include <math.h>
#include "iterator.h"
#include "error.h"

struct deque;

typedef struct deque* deque_t;

/**
 * Initializes new deque
 * @return new deque
 */ 
deque_t deque_init();

int deque_destroy(deque_t deque);

size_t deque_size(deque_t deque);

/**
 * Pushes element to end of collection
 * @param deque deque on which operation will be performed
 * @param element element to be pushed to back
 * @return 0 if operation was successful
 */ 
int deque_push_back(deque_t deque, void *element);

/**
 * Returns value of last element
 * @param deque deque on which operation will be performed
 * @return value of last element
 */ 
void *deque_back(deque_t deque);

/**
 * Pushes element to front of collection
 * @param deque deque on which operation will be performed
 * @param element element to be inserted
 * @return 0 if operation was successful
 */ 
int deque_push_front(deque_t deque, void *element);

/**
 * Returns value of first element
 * @param deque deque on which operation will be performed
 * @return value of first element
 */ 
void *deque_front(deque_t deque);

/**
 * Pops last element
 * @param deque deque on which operation will be performed
 * @return 0 if popped successfully
 */ 
int deque_pop_back(deque_t deque);

/**
 * Pops first element
 * @param deque deque on which operation will be performed
 * @return 0 if popped successfully
 */ 
int deque_pop_front(deque_t deque);

/**
 * Returns iterator to first element
 * @param deque deque on which operation will be performed
 * @return iterator to beginning
 */ 
iterator_t deque_begin(deque_t deque);

/**
 * Returns iterator to last element
 * @param deque deque on which operation will be performed
 * @return iterator to end
 */ 
iterator_t deque_end(deque_t deque);

/**
 * Returns reversed iterator to first element
 * @param deque deque on which operation will be performed
 * @return iterator to beginning
 */ 
iterator_t deque_rbegin(deque_t deque);

/**
 * Returns reversed iterator to last element
 * @param deque deque on which operation will be performed
 * @return iterator to end
 */ 
iterator_t deque_rend();

/**
 * Returns value of element
 * @param ite iterator
 * @return value of element
 */ 
void *deque_iterator_value(iterator_t ite);

#endif
