#ifndef __H_ITERATOR
#define __H_ITERATOR

#include <stdlib.h>
#include <stdbool.h>

struct iterator
{
    void *current_element;
    size_t position;
    struct iterator (*advance)(struct iterator ite, int distance);
};

typedef struct iterator iterator_t;
typedef iterator_t (*advance_function_t)(iterator_t ite, int distance);

iterator_t iterator_init(void *current_element, size_t position, advance_function_t advance);

iterator_t iterator_next(iterator_t ite);
iterator_t iterator_prev(iterator_t ite);
iterator_t iterator_advance(iterator_t ite, int distance);
size_t iterator_distance(iterator_t start, iterator_t end);

void *iterator_value(iterator_t it);

bool iterator_equal(iterator_t a, iterator_t b);

#endif
