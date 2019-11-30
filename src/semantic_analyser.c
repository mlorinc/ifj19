#include <stdio.h>
#include <assert.h>
#include "semantic_analyser.h"
#include "deque.h"
#include "symtable.h"
#include "ptr_string.h"

semantic_result_t semantic_result(ast_t ast, scope_t scope, enum error_codes status)
{
    semantic_result_t sem;
    sem.ast = ast;
    sem.scope = scope;
    sem.status = status;
    return sem;
}

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

bool set_function(scope_t scope, ptr_string_t key, void *value)
{
    return set_variable_in_scope(scope->root_scope, key, value);
}

semantic_result_t handle_fuction_definition(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    // todo: supports inner function, remove if it is not in ifj assignment
    // get function name
    ptr_string_t function_name = node->data;

    // check if it already exists
    if (exists_variable_in_scope(current_scope->root_scope, function_name))
    {
        // function redefinition is not allowed
        char *variable = ptr_string_c_string(function_name);
        ast_t func_node = hash_map_get(current_scope->root_scope->local_table, variable);
        fprintf(stderr, "Function %s is already defined in scope (line: %d, first defined line: %u)\n", variable, node->line, func_node->line);
        free(variable);
        return semantic_result(node, current_scope, ERROR_SEM);
    }
    else
    {
        // ok, we are in function, create new scope
        scope_t scope = new_scope(current_scope, node);

        // extract function arguments
        ast_t parameters = array_nodes_get(node->nodes, 0);

        // extract function body
        ast_t body = array_nodes_get(node->nodes, 1);

        deque_push_back(tree_traversing_deque, ast_node_init(LEAVE_SCOPE, 0, 0, NULL));

        // make sure types are exact
        assert(parameters->node_type == FUNCTION_PARAMETERS);
        assert(body->node_type == CONSEQUENT);

        enum error_codes status = ERROR_OK;

        // argument shadowing is supported, so lets them add to new function scope
        for (size_t i = 0; i < array_nodes_size(parameters->nodes); i++)
        {
            // add parameters to local variable symbol table
            ast_t arg = array_nodes_get(parameters->nodes, i);
            char *key = ptr_string_c_string(arg->data);

            // repeating parameter, thats error
            if (hash_map_has(scope->local_table, key))
            {
                status = ERROR_SEM;
            }
            else
            {
                hash_map_put(scope->local_table, key, arg);
            }
            free(key);
        }

        set_function(current_scope, function_name, node);

        // send body of function to be checked semantically
        deque_push_back(tree_traversing_deque, body);

        // return new function scope
        return semantic_result(node, scope, status);
    }
}

semantic_result_t handle_assign(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    // extract id from assign
    ast_t id = array_nodes_get(node->nodes, 0);
    // extract expression from assign
    ast_t expression = array_nodes_get(node->nodes, 1);

    // find scope, where id is defined
    scope_t scope = find_scope_with_defined_variable(current_scope, id->data);

    char *key = ptr_string_c_string(id->data);

    enum error_codes status = ERROR_OK;

    // if id is not defined, define it in current scope
    if (scope == NULL)
    {
        set_variable_in_scope(current_scope, id->data, id);
    }
    else
    {
        // if it is, check if it is not function
        ast_t variable_node = hash_map_get(scope->local_table, key);

        if (variable_node->node_type == FUNCTION_DEFINITION)
        {
            status = ERROR_SEM;
            fprintf(stderr, "Variable %s cannot have same name as declared function (variable line: %u, function line: %u)\n", key, id->line, variable_node->line);
        }
        else
        {
            hash_map_put(scope->local_table, key, id);
        }
    }

    free(key);

    // send expression for semantic test
    deque_push_back(tree_traversing_deque, expression);
    // scope did not change
    return semantic_result(node, current_scope, status);
}

semantic_result_t handle_while(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    // extract condition from while
    ast_t condition = array_nodes_get(node->nodes, 0);
    // extract body from while
    ast_t body = array_nodes_get(node->nodes, 1);
    // extract else clause which is optional
    ast_t else_clause = array_nodes_try_get(node->nodes, 2);

    deque_push_back(tree_traversing_deque, ast_node_init(LEAVE_SCOPE, 0, 0, NULL));

    if (else_clause != NULL)
    {
        deque_push_back(tree_traversing_deque, else_clause);
    }

    deque_push_back(tree_traversing_deque, body);
    deque_push_back(tree_traversing_deque, condition);

    return semantic_result(node, new_scope(current_scope, node), ERROR_OK);
}

semantic_result_t handle_if(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    // extract condition from while
    ast_t condition = array_nodes_get(node->nodes, 0);
    // extract body from while
    ast_t body = array_nodes_get(node->nodes, 1);
    // extract else clause which is optional
    ast_t alternate_clause = array_nodes_try_get(node->nodes, 2);

    if (alternate_clause != NULL)
    {
        deque_push_back(tree_traversing_deque, alternate_clause);
    }

    deque_push_back(tree_traversing_deque, ast_node_init(LEAVE_SCOPE, 0, 0, NULL));
    deque_push_back(tree_traversing_deque, body);
    deque_push_back(tree_traversing_deque, condition);

    return semantic_result(node, new_scope(current_scope, node), ERROR_OK);
}

semantic_result_t handle_else(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    ast_t body = array_nodes_get(node->nodes, 0);
    deque_push_back(tree_traversing_deque, ast_node_init(LEAVE_SCOPE, 0, 0, NULL));
    deque_push_back(tree_traversing_deque, body);
    return semantic_result(node, new_scope(current_scope, node), ERROR_OK);
}

semantic_result_t handle_return(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    // we must ensure, return is in function
    scope_t function_scope = find_first_node_type_in_scope(current_scope, FUNCTION_DEFINITION);
    enum error_codes status = ERROR_OK;

    if (function_scope == NULL)
    {
        fprintf(stderr, "return is not inside function (line %u)\n", node->line);
        status = ERROR_SYNTAX;
    }

    ast_t expr = array_nodes_get(node->nodes, 0);

    if (expr->node_type != NONE)
    {
        // it is valid expresssion, check it
        deque_push_back(tree_traversing_deque, expr);
    }

    return semantic_result(node, current_scope, status);
}

semantic_result_t handle_break_continue(scope_t current_scope, ast_t node, const char *what)
{
    // we must ensure, return is in function
    scope_t while_scope = find_first_node_type_in_scope(current_scope, WHILE);
    enum error_codes status = ERROR_OK;

    if (while_scope == NULL)
    {
        fprintf(stderr, "%s is not inside while (line %u)\n", what, node->line);
        status = ERROR_SYNTAX;
    }
    return semantic_result(node, current_scope, status);
}

semantic_result_t handle_consequent(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    array_nodes_t nodes = node->nodes;
    size_t size = array_nodes_size(nodes);

    // todo: verify if this code is reachable
    if (size == 0)
    {
        return semantic_result(node, current_scope, ERROR_OK);
    }

    for (size_t i = size - 1; i > 0; i--)
    {
        deque_push_back(tree_traversing_deque, array_nodes_get(nodes, i));
    }

    deque_push_back(tree_traversing_deque, array_nodes_get(nodes, 0));

    return semantic_result(node, current_scope, ERROR_OK);
}

/**
 * Returns error count so far
 */
semantic_result_t handle_node(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    switch (node->node_type)
    {
    case LEAVE_SCOPE:
        return semantic_result(node, delete_scope(current_scope), ERROR_OK);

    case PASS:
        return semantic_result(node, current_scope, ERROR_OK);

    case RETURN:
        // todo: validate expression
        return handle_return(current_scope, node, tree_traversing_deque);
    case CONTINUE:
        return handle_break_continue(current_scope, node, "continue");

    case BREAK:
        return handle_break_continue(current_scope, node, "break");

    case FUNCTION_DEFINITION:
        return handle_fuction_definition(current_scope, node, tree_traversing_deque);

    case ASSIGN:
        return handle_assign(current_scope, node, tree_traversing_deque);

    case WHILE:
        return handle_while(current_scope, node, tree_traversing_deque);

    case IF:
    case ELIF:
        return handle_if(current_scope, node, tree_traversing_deque);

    case ELSE:
        return handle_else(current_scope, node, tree_traversing_deque);

    case NONE:
        return semantic_result(node, current_scope, ERROR_OK);

    case EXPRESSION:
        return semantic_result(node, current_scope, ERROR_INTERNAL);;

    case CONSEQUENT:
        return handle_consequent(current_scope, node, tree_traversing_deque);

    default:
        fprintf(stderr, "Forgot to implement %d on line %u\n", node->node_type, node->line);
        return semantic_result(node, current_scope, ERROR_INTERNAL);
    }
}

semantic_result_t semantic_analysis(ast_t root)
{
    if (root == NULL) {
        return semantic_result(NULL, NULL, ERROR_OK);
    }

    scope_t current_scope = new_scope(NULL, root);
    
    deque_t tree_traversing_deque = deque_init();
    deque_push_back(tree_traversing_deque, root);

    enum error_codes status = ERROR_OK;

    while (deque_size(tree_traversing_deque) > 0)
    {
        ast_t node = deque_pop_back(tree_traversing_deque);
        semantic_result_t result = handle_node(current_scope, node, tree_traversing_deque);
        current_scope = result.scope;

        if (status != ERROR_SYNTAX && status != ERROR_INTERNAL)
        {
            status = result.status;
        }
    }

    deque_destroy(tree_traversing_deque);

    // we should bubble up to root
    assert(current_scope->root == root);

    delete_scope(current_scope);
    return semantic_result(root, NULL, status);
}
