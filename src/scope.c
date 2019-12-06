#include <stdlib.h>
#include "scope.h"

scope_t new_scope(scope_t parent, ast_t root)
{
    scope_t scope = malloc(sizeof(struct scope));
    scope->root = root;
    scope->local_table = hash_map_init();
    scope->parent_scope = parent;

    if (parent == NULL) {
        scope->root_scope = scope;
    }
    else {
        scope->root_scope = parent->root_scope;
    }
    return scope;
}

scope_t delete_scope(scope_t scope)
{
    hash_map_destroy(scope->local_table);
    scope_t parent = scope->parent_scope;
    free(scope);
    return parent;
}

scope_t find_scope_with_defined_variable(scope_t scope, ptr_string_t key)
{
    scope_t current_scope = scope;

    while (current_scope != NULL)
    {
        char *raw_key = ptr_string_c_string(key);

        if (hash_map_has(current_scope->local_table, raw_key))
        {
            return current_scope;
        }
        else
        {
            current_scope = current_scope->parent_scope;
        }
        free(raw_key);
    }

    return NULL;
}

scope_t find_first_node_type_in_scope(scope_t scope, ast_node_type_t type)
{
    scope_t current_scope = scope;

    while (current_scope != NULL)
    {
        ast_node_type_t node_type = current_scope->root->node_type;

        if (type == node_type)
        {
            return current_scope;
        }
        else
        {
            current_scope = current_scope->parent_scope;
        }
    }

    return NULL;
}

bool exists_variable_in_scope(scope_t scope, ptr_string_t key)
{
    return find_scope_with_defined_variable(scope, key) != NULL;
}

bool set_variable_in_scope(scope_t scope, ptr_string_t key, void *value)
{
    scope_t variable_scope;
    scope_t func_scope = find_first_node_type_in_scope(scope, FUNCTION_DEFINITION);

    // we are in global scope
    if (func_scope == NULL) {
        variable_scope = scope->root_scope;
    }
    else {
        variable_scope = func_scope;
    }

    char *variable_key = ptr_string_c_string(key);
    bool result = hash_map_put(variable_scope->local_table, variable_key, value);
    free(variable_key);

    return result;
}

bool set_function_in_scope(scope_t scope, ptr_string_t key, void *value)
{
    return set_variable_in_scope(scope->root_scope, key, value);
}
