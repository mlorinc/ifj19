#ifndef __H_ARRAY_NODES
#define __H_ARRAY_NODES

#include <stddef.h>
#include <stdbool.h>
#include "parser_ast.h"

#ifndef __ARRAY_CHUNK
#define __ARRAY_CHUNK 6
#endif

struct array_nodes;

typedef struct array_nodes *array_nodes_t;

array_nodes_t array_nodes_init();

size_t array_nodes_size(array_nodes_t array);
size_t array_nodes_capacity(array_nodes_t array);
ast_t array_nodes_get(array_nodes_t array, size_t index);
bool array_nodes_set(array_nodes_t array, size_t index, ast_t node);
bool array_nodes_push(array_nodes_t array, ast_t node);
bool array_nodes_destroy(array_nodes_t array);

#endif
