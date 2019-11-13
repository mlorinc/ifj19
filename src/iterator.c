#include "iterator.h"

iterator_t iterator_init(void *current_element, size_t position, advance_function_t advance) {
    iterator_t ite;

    ite.current_element = current_element;
    ite.advance = advance;
    ite.position = position;
    
    return ite;
}

iterator_t iterator_next(iterator_t ite) {
    return ite.advance(ite, 1);
}

iterator_t iterator_prev(iterator_t ite) {
    return ite.advance(ite, -1);
}

iterator_t iterator_advance(iterator_t ite, int distance) {
    return ite.advance(ite, distance);
}

size_t iterator_distance(iterator_t start, iterator_t end) {
    return abs(end.position - start.position);
}

bool iterator_equal(iterator_t a, iterator_t b) {
    return a.position == b.position;
}

void *iterator_value(iterator_t it) {
    return it.current_element;
}
