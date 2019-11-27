#include <stdlib.h>
#include "array_nodes.h"
#include "string.h"

struct array_nodes
{
    size_t capacity;
    size_t size;
    ast_t *nodes;
};

array_nodes_t array_nodes_init() {
    array_nodes_t array = malloc(sizeof(struct array_nodes));
    array->nodes = malloc(sizeof(ast_t) * __ARRAY_CHUNK);

    array->capacity = __ARRAY_CHUNK;
    array->size = 0;
}

size_t array_nodes_size(array_nodes_t array) {
    return array->size;
}

size_t array_nodes_capacity(array_nodes_t array) {
    return array->capacity;
}

ast_t array_nodes_get(array_nodes_t array, size_t index) {
    assert(index < array->size);
    return array->nodes[index];
}

bool array_nodes_set(array_nodes_t array, size_t index, ast_t node) {
    assert(index < array->size);
    array->nodes[index] = node;
}

bool array_nodes_push(array_nodes_t array, ast_t node) {
    if (array->size >= array->capacity) {
        size_t new_capacity = array->capacity + __ARRAY_CHUNK;
        array->nodes = realloc(array->nodes, sizeof(ast_t) * new_capacity;
        array->capacity = new_capacity;
    }
    array->nodes[array->size] = node;
}

bool array_nodes_destroy(array_nodes_t array) {
    free(array->nodes);
    free(array);
}
