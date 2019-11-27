#ifndef __H_AST_STRUCTURES
#define __H_AST_STRUCTURES

#include "parser_ast.h"
#include "ptr_string.h"

typedef struct ast_if {
    ast_t condition;
    ast_t body;
    ast_t alternate;
} ast_if_t;

typedef struct ast_if ast_while_t;

typedef struct ast_function_definition {
    ptr_string_t identifer;
    ast_t parameters;
    ast_t body;
} ast_function_definition_t;

typedef struct ast_function_call {
    ptr_string_t identifer;
    ast_t parameters;
} ast_function_call_t;

typedef struct ast_expression {
    ast_node_type_t node_type;
    ast_t first;
    ast_t second;
} ast_expression_t;

typedef struct ast_assign {
    ptr_string_t id;
    ast_t init;
} ast_expression_t;



#endif
