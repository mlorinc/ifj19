#include <stdio.h>
#include <assert.h>
#include "semantic_analyser.h"
#include "deque.h"
#include "symtable.h"
#include "ptr_string.h"
#include "scaner.h"
#include "stack.h"

typedef struct scope
{
    ast_t root;
    hash_map_t local_table;
    struct scope *parent_scope;
} * scope_t;

scope_t new_scope(scope_t parent, ast_t root)
{
    scope_t scope = malloc(sizeof(struct scope));
    scope->root = root;
    scope->local_table = hash_map_init();
    scope->parent_scope = parent;
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

bool set_variable_in_scope(scope_t scope, const char *key, void *value)
{
    return hash_map_put(scope->local_table, key, value);
}

void print_undefined_variable_error(ptr_string_t variable_name)
{
    char *variable = ptr_string_c_string(variable_name);
    fprintf(stderr, "Variable %s is undefined\n", variable);
    free(variable);
}

scope_t handle_fuction_definition(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    // todo: supports inner function, remove if it is not in ifj assignment
    // get function name
    ptr_string_t function_name = node->data;

    // check if it already exists
    if (exists_variable_in_scope(current_scope, function_name))
    {
        // function redefinition is not allowed
        char *variable = ptr_string_c_string(function_name);
        fprintf(stderr, "Function %s is already defined in scope\n", variable);
        free(variable);
        return current_scope;
    }
    else
    {
        // ok, we are in function, create new scope
        scope_t scope = new_scope(current_scope, node);

        // extract function arguments
        ast_t parameters = array_nodes_get(node->nodes, 0);

        // extract function body
        ast_t body = array_nodes_get(node->nodes, 1);

        deque_push_back(tree_traversing_deque, ast_node_init(LEAVE_SCOPE, NULL));

        // make sure types are exact
        assert(parameters->node_type == FUNCTION_PARAMETERS);
        assert(body->node_type == CONSEQUENT);

        // argument shadowing is supported, so lets them add to new function scope
        for (size_t i = 0; i < array_nodes_size(parameters->nodes); i++)
        {
            // add parameters to local variable symbol table
            ast_t arg = array_nodes_get(parameters->nodes, i);
            char *key = ptr_string_c_string(arg->data);
            hash_map_put(scope->local_table, key, arg);
            free(key);
        }

        // send body of function to be checked semantically
        deque_push_back(tree_traversing_deque, body);

        // return new function scope
        return scope;
    }
}

scope_t handle_assign(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    // extract id from assign
    ast_t id = array_nodes_get(node->nodes, 0);
    // extract expression from assign
    ast_t expression = array_nodes_get(node->nodes, 1);

    // find scope, where id is defined
    scope_t scope = find_scope_with_defined_variable(current_scope, id->data);

    // if id is not defined, define it in current scope
    if (scope == NULL)
    {
        hash_map_put(current_scope->local_table, id->data, NULL);
    }
    else
    {
        // if it is overwrite it in found scope
        hash_map_put(scope->local_table, id->data, NULL);
    }
    // send expression for semantic test
    deque_push_back(tree_traversing_deque, expression);
    // scope did not change
    return current_scope;
}

scope_t handle_while(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    // extract condition from while
    ast_t condition = array_nodes_get(node->nodes, 0);
    // extract body from while
    ast_t body = array_nodes_get(node->nodes, 1);
    // extract else clause which is optional
    ast_t else_clause = array_nodes_try_get(node->nodes, 2);

    deque_push_back(tree_traversing_deque, ast_node_init(LEAVE_SCOPE, NULL));

    if (else_clause != NULL)
    {
        deque_push_back(tree_traversing_deque, else_clause);
    }

    deque_push_back(tree_traversing_deque, body);
    deque_push_back(tree_traversing_deque, condition);

    return new_scope(current_scope, node);
}

scope_t handle_if(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
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

    deque_push_back(tree_traversing_deque, ast_node_init(LEAVE_SCOPE, NULL));
    deque_push_back(tree_traversing_deque, body);
    deque_push_back(tree_traversing_deque, condition);

    return new_scope(current_scope, node);
}

scope_t handle_else(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    ast_t body = array_nodes_get(node->nodes, 0);
    deque_push_back(tree_traversing_deque, ast_node_init(LEAVE_SCOPE, NULL));
    deque_push_back(tree_traversing_deque, body);
    return new_scope(current_scope, node);
}

scope_t handle_return(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    // we must ensure, return is in function
    scope_t function_scope = find_first_node_type_in_scope(current_scope, FUNCTION_DEFINITION);

    if (function_scope == NULL)
    {
        fprintf(stderr, "return is not inside function\n");
    }

    ast_t expr = array_nodes_get(node->nodes, 0);

    if (expr->node_type != NONE)
    {
        // it is valid expresssion, check it
        deque_push_back(tree_traversing_deque, expr);
    }

    return current_scope;
}

scope_t handle_break_continue(scope_t current_scope, const char *what)
{
    // we must ensure, return is in function
    scope_t while_scope = find_first_node_type_in_scope(current_scope, WHILE);

    if (while_scope == NULL)
    {
        fprintf(stderr, "%s is not inside while\n", what);
    }
    return current_scope;
}

scope_t handle_consequent(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    array_nodes_t nodes = node->nodes;
    size_t size = array_nodes_size(nodes);

    // todo: verify if this code is reachable
    if (size == 0)
    {
        return current_scope;
    }

    for (size_t i = size - 1; i > 0; i--)
    {
        deque_push_back(tree_traversing_deque, array_nodes_get(nodes, i));
    }

    deque_push_back(tree_traversing_deque, array_nodes_get(nodes, 0));

    return current_scope;
}

typedef enum expressionItem
{
    string_e,
    int_e,
    float_e,
    add_e,
    sub_e,
    mul_e,
    div_e,
    floordiv_e,
    compare_e,
    bool_e
}* expreItem_p;

expreItem_p convert_token_to_expreItem(tToken_type token)
{
    expreItem_p item = malloc(sizeof(enum expressionItem));
    switch(token)
    {
        case TSTRING:
            *item = string_e;
            break;
        case TINT:
            *item = int_e;
            break;
        case TFLOAT:
            *item = float_e;
            break;
        case TADD:
            *item = add_e;
            break;
        case TSUB:
            *item = sub_e;
            break;
        case TMUL:
            *item = mul_e;
            break;
        case TDIV:
            *item = div_e;
            break;
        case TFLOORDIV:
            *item = floordiv_e;
            break;
        default:
            *item = compare_e;
    }
    return item;
}

int do_operation(stack_t stack, enum expressionItem operation)
{
    expreItem_p first = stack_pop(stack);
    expreItem_p second = stack_pop(stack);
    switch (operation)
    {
    case add_e:
        if(*first == string_e && *second == string_e)
        {
            free(first);
            stack_push(stack, second);
            return 0;
        }
    case sub_e:
    case mul_e:
        if(*first == int_e && *second == int_e)
        {
            free(first);
            stack_push(stack, second);
            return 0;
        }
        if(*first == int_e && *second == float_e ||
            *first == float_e && *second == int_e ||
            *first == float_e && *second == float_e)
        {
            free(first);
            second = float_e;
            stack_push(stack, second);
            return 0;
        }
        break;
    case div_e:
        if(*first == int_e || *first == float_e &&
            *second == int_e || *second == float_e)
        {
            free(first);
            second = float_e;
            stack_push(stack, second);
            return 0;
        }
        break;
    case floordiv_e:
        if(*first == int_e && *second == int_e)
        {
            free(first);
            stack_push(stack, second);
            return 0;
        }
        break;
    case compare_e:
        if(*first != bool_e && *second != bool_e)
        {
            free(first);
            second = bool_e;
            stack_push(stack, second);
            return 0;
        }
        break;
    }
    free(first);
    free(second);
    return 1;
}

scope_t handle_expression(scope_t current_scope, queue_t postfix)
{
    tToken* token;
    queue_t checkedPostfix = queue_init();
    stack_t semanticStack = stack_init();
    expreItem_p item = NULL;

    while (!queue_empty(postfix))
    {
        token = queue_pop(postfix);
        queue_push(checkedPostfix, token);
        if(token->type == TIDENTIFICATOR)
        {
            if (!exists_variable_in_scope(current_scope, token->value))
            {
                print_undefined_variable_error(token->value);
                return current_scope;
            }
            //TODO convert identificator
        }
        else
        {
            item = convert_token_to_expreItem(token->type);
            if(*item == string_e || *item == int_e || *item == float_e)
            {
                stack_push(semanticStack, item);
            }
            else
            {
                if(do_operation(semanticStack, *item) != 0)
                {
                    free(item);
                    stack_destroy(semanticStack);
                    queue_destroy(checkedPostfix);
                    fprintf(stderr, "Syntactic error in expression.");
                    return current_scope;
                }
            }
        }
    }
    if(item != NULL)
    {
        free(item);
    }
    stack_destroy(semanticStack);
    while (!queue_empty(checkedPostfix))
    {
        queue_push(postfix, queue_pop(checkedPostfix));
    }
    queue_destroy(checkedPostfix);
    return current_scope;
}

/**
 * Returns error count so far
 */
scope_t handle_node(scope_t current_scope, ast_t node, deque_t tree_traversing_deque)
{
    switch (node->node_type)
    {
    case LEAVE_SCOPE:
        return delete_scope(current_scope);

    case PASS:
        return current_scope;

    case RETURN:
        // todo: validate expression
        return handle_return(current_scope, node, tree_traversing_deque);
    case CONTINUE:
        return handle_break_continue(current_scope, "continue");

    case BREAK:
        return handle_break_continue(current_scope, "break");

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
        return current_scope;

    case EXPRESSION:
        return handle_expression(current_scope, node->data);

    case CONSEQUENT:
        return handle_consequent(current_scope, node, tree_traversing_deque);

    default:
        fprintf(stderr, "Forgot to implement %d\n", node->node_type);
        break;
    }

    return current_scope;
}

void semantic_analysis(ast_t root)
{
    scope_t current_scope = new_scope(NULL, root);

    deque_t tree_traversing_deque = deque_init();
    deque_push_back(tree_traversing_deque, root);

    while (deque_size(tree_traversing_deque) > 0)
    {
        ast_t node = deque_pop_back(tree_traversing_deque);
        current_scope = handle_node(current_scope, node, tree_traversing_deque);
    }

    deque_destroy(tree_traversing_deque);

    // we should bubble up to root
    assert(current_scope->root == root);

    delete_scope(current_scope);
}
