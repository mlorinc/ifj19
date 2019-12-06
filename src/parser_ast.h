#ifndef __H_PARSER_AST
#define __H_PARSER_AST

#include <stdbool.h>
#include "queue.h"

typedef enum {
    LEAVE_SCOPE,
    PASS,
    RETURN,
    BREAK,
    CONTINUE,
    FUNCTION,
    FUNCTION_DEFINITION,
    FUNCTION_PARAMETERS,
    ID,
    CONSTANT,
    PLUS_OP,
    MINUS_OP,
    MULTIPLY_OP,
    DIVIDE_OP,
    FLOOR_DIVIDE_OP,
    EQUALS,
    ASSIGN,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_EQUAL,
    GREATER_THAN_EQUAL,
    NOT_EQUAL,
    WHILE,
    IF,
    ELIF,
    ELSE,
    WHILE_ELSE,
    NONE,
    EXPRESSION,
    CONSEQUENT
} ast_node_type_t;

struct array_nodes;

struct ast
{
    struct array_nodes *nodes;
    ast_node_type_t node_type;
    void *data;
};


typedef struct ast *ast_t;

ast_t ast_node_init_empty();

ast_t ast_node_init(ast_node_type_t type, void *data);

bool ast_delete(ast_t root);

bool ast_add_node(ast_t parent, ast_t node);

void *allocate_value_to_heap(void *value, size_t size);

#include "array_nodes.h"

#endif
