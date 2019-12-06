#ifndef __H_SCOPE
#define __H_SCOPE

#include <stdbool.h>
#include "symtable.h"
#include "parser_ast.h"
#include "ptr_string.h"

typedef struct scope
{
    ast_t root;
    hash_map_t local_table;
    struct scope *parent_scope;
    struct scope *root_scope;
} * scope_t;

scope_t new_scope(scope_t parent, ast_t root);
scope_t delete_scope(scope_t scope);
scope_t find_scope_with_defined_variable(scope_t scope, ptr_string_t key);
scope_t find_first_node_type_in_scope(scope_t scope, ast_node_type_t type);
scope_t get_function_scope(scope_t scope);
bool exists_variable_in_scope(scope_t scope, ptr_string_t key);
bool set_variable_in_scope(scope_t scope, ptr_string_t key, void *value);
bool set_function_in_scope(scope_t scope, ptr_string_t key, void *value);

#endif
