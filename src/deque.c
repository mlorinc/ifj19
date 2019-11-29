#include "deque.h"

typedef struct deque_item
{
    struct deque_item *next;
    struct deque_item *prev;
    void *value;
} * deque_item_t;

struct deque
{
    deque_item_t first;
    deque_item_t last;
    size_t size;
};

deque_t deque_init()
{
    deque_t deque = calloc(1, sizeof(struct deque));
    if (deque != NULL)
    {
        deque->size = 0;
    }
    return deque;
}

size_t deque_size(deque_t deque)
{
    return deque->size;
}

bool deque_destroy(deque_t deque)
{
    deque_item_t current = deque->first;

    while (current != NULL)
    {
        deque_item_t oldFirst = current;
        current = current->next;
        free(oldFirst->value);
        free(oldFirst);
    }
    free(deque);
    return true;
}

bool deque_push_back(deque_t deque, void *element)
{
    deque_item_t node = calloc(1, sizeof(struct deque_item));

    if (node == NULL)
    {
        return false;
    }

    if (deque->size == 0)
    {
        deque->last = node;
        deque->first = node;
    }
    else
    {
        node->prev = deque->last;
        deque->last->next = node;
        deque->last = node;
    }

    node->value = element;
    deque->size++;
    return true;
}

void *deque_back(deque_t deque)
{
    return deque->last != NULL ? deque->last->value : NULL;
}

bool deque_push_front(deque_t deque, void *element)
{
    deque_item_t node = calloc(1, sizeof(struct deque_item));

    if (node == NULL)
    {
        return false;
    }

    if (deque->size == 0)
    {
        deque->last = node;
        deque->first = node;
    }
    else
    {
        node->next = deque->first;
        deque->first->prev = node;
        deque->first = node;
    }

    node->value = element;
    deque->size++;
    return true;
}

void *deque_front(deque_t deque)
{
    return deque->first != NULL ? deque->first->value : NULL;
}

void *deque_pop_back(deque_t deque)
{
    if (deque->size == 0)
    {
        return NULL;
    }

    void *popped_element = deque->last->value;

    if (deque->size == 1)
    {
        free(deque->first);
        deque->first = NULL;
        deque->last = NULL;
    }
    else
    {
        deque_item_t prev = deque->last->prev;
        prev->next = NULL;

        free(deque->last);

        deque->last = prev;
    }
    deque->size--;
    return popped_element;
}

void *deque_pop_front(deque_t deque)
{
    if (deque->size == 0)
    {
        return NULL;
    }

    void *popped_element = deque->first->value;

    if (deque->size == 1)
    {
        free(deque->first);
        deque->first = NULL;
        deque->last = NULL;
    }
    else
    {
        deque_item_t next = deque->first->next;
        next->prev = NULL;

        free(deque->first);
        deque->first = next;
    }
    deque->size--;
    return popped_element;
}

iterator_t advance(iterator_t ite, int distance)
{
    if (distance == 0)
    {
        return ite;
    }
    else if (distance > 0)
    {
        for (int i = 0; i < distance; i++)
        {
            deque_item_t node = iterator_value(ite);
            ite.current_element = node->next;
            ite.position++;
        }
    }
    else
    {
#include <stdio.h>
        size_t pos_distance = -distance;

        if (ite.position < pos_distance)
        {
            return ite;
        }

        for (size_t i = ite.position, end = ite.position - pos_distance; i > end; i--)
        {
            deque_item_t node = iterator_value(ite);
            ite.current_element = node->prev;
            ite.position--;
        }
    }
    return ite;
}

iterator_t deque_begin(deque_t deque)
{
    return iterator_init(deque->first, 0, advance);
}

iterator_t deque_end(deque_t deque)
{
    return iterator_init(NULL, deque->size, advance);
}

iterator_t deque_rbegin(deque_t deque)
{
    return iterator_init(deque->last, deque->size - 1, advance);
}

iterator_t deque_rend()
{
    return iterator_init(NULL, -1, advance);
}

void *deque_iterator_value(iterator_t ite)
{
    deque_item_t item = iterator_value(ite);
    return item->value;
}
