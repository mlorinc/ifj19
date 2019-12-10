#include "generator.h"
#include "generator_functions.h"
#include "stack.h"
#include "scope.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct generator_result
{
    scope_t current_scope;
    enum error_codes status;
} generator_result_t;

generator_result_t generator_result(scope_t current_scope, enum error_codes status)
{
    generator_result_t result;
    result.current_scope = current_scope;
    result.status = status;
    return result;
}

generator_result_t handle_consequent(scope_t current_scope, ast_t node, stack_t tree_traversal)
{
    array_nodes_t nodes = node->nodes;
    size_t size = array_nodes_size(nodes);

    // todo: verify if this code is reachable
    if (size == 0)
    {
        return generator_result(current_scope, ERROR_OK);
    }

    for (size_t i = size - 1; i > 0; i--)
    {
        stack_push(tree_traversal, array_nodes_get(nodes, i));
    }

    stack_push(tree_traversal, array_nodes_get(nodes, 0));

    return generator_result(current_scope, ERROR_OK);
}

char *get_label_name(ast_node_type_t type)
{
    switch (type)
    {
    case IF:
        return "IF";
    case ELIF:
        return "ELIF";
    case ELSE:
        return "ELSE";
    default:
        fprintf(stderr, "Unsupported type conversion on line %d", __LINE__);
        break;
    }

    return NULL;
}

void handle_conditions(scope_t scope, ast_t node)
{
    ast_t current_node = node;
    bool has_else = false;

    do
    {
        ast_t condition = array_nodes_try_get(node->nodes, 0);
        ast_t alternate_clause = array_nodes_try_get(node->nodes, 2);

        if (condition != NULL)
        {
            generate_condition(scope, condition->data, get_label_name(current_node->node_type), current_node->line);
        }
        else
        {
            // thats else
            generate_condition(scope, NULL, get_label_name(current_node->node_type), current_node->line);
            has_else = true;
        }

        current_node = alternate_clause;
    } while (current_node != NULL);

    if (!has_else)
    {
        // jump out of if, elif bodies
        generate_endif_jump(node->line);
    }
}

generator_result_t handle_if_elif_else(scope_t current_scope, ast_t node, stack_t tree_traversal)
{
    scope_t scope = new_scope(current_scope, node);

    ast_t body = array_nodes_try_get(node->nodes, 1);
    ast_t alternate_clause = array_nodes_try_get(node->nodes, 2);

    if (alternate_clause != NULL)
    {
        stack_push(tree_traversal, alternate_clause);
    }

    stack_push(tree_traversal, ast_node_init(LEAVE_SCOPE, 0, 0, NULL));
    stack_push(tree_traversal, body);

    return generator_result(scope, ERROR_OK);
}

generator_result_t handle_if(scope_t current_scope, ast_t node, stack_t tree_traversal)
{
    ast_t leave_if = ast_node_init(LEAVE_IF, node->line, node->pos, NULL);
    scope_t helper_scope = new_scope(current_scope, leave_if);

    // this will be popped when we process all ifs
    stack_push(tree_traversal, leave_if);

    return handle_if_elif_else(helper_scope, node, tree_traversal);
}

generator_result_t handle_return(scope_t current_scope, ast_t node)
{
    ast_t expr = array_nodes_try_get(node->nodes, 0);
    generate_return(current_scope, expr->node_type == NONE ? NULL : expr->data);
    return generator_result(current_scope, ERROR_OK);
}

generator_result_t handle_function_definition(scope_t current_scope, ast_t node, stack_t tree_traversal)
{
    scope_t function_scope = new_scope(current_scope, node);

    char *id = ptr_string_c_string(node->data);
    ast_t parameters = array_nodes_get(node->nodes, 0);
    ast_t body = array_nodes_get(node->nodes, 1);

    generate_function_header(id, parameters->nodes);
    free(id);

    // leave scope
    stack_push(tree_traversal, ast_node_init(LEAVE_SCOPE, 0, 0, NULL));

    stack_push(tree_traversal, body);
    return generator_result(function_scope, ERROR_OK);
}

generator_result_t handle_function_call(scope_t current_scope, ast_t node)
{
    generate_function_call(current_scope, node);
    return generator_result(current_scope, ERROR_OK);
}

generator_result_t handle_assignment(scope_t current_scope, ast_t node)
{
    ast_t lvalue = array_nodes_get(node->nodes, 0);
    ast_t rvalue = array_nodes_get(node->nodes, 1);

    char *id = ptr_string_c_string(lvalue->data);

    if (rvalue->node_type == FUNCTION_CALL)
    {
        generate_function_call_assignment(current_scope, id, rvalue);
    }
    else
    {
        generate_expression_assignment(current_scope, id, rvalue);
    }

    free(id);
    return generator_result(current_scope, ERROR_OK);
}

generator_result_t handle_while(scope_t current_scope, ast_t node, stack_t tree_traversal)
{
    scope_t scope = new_scope(current_scope, node);

    stack_push(tree_traversal, ast_node_init(LEAVE_SCOPE, 0, 0, NULL));

    ast_t condition = array_nodes_get(node->nodes, 0);
    ast_t body = array_nodes_get(node->nodes, 1);
    ast_t else_statement = array_nodes_try_get(node->nodes, 2);

    generate_while_label(node->line);
    generate_while_condition(scope, condition->data);

    stack_push(tree_traversal, ast_node_init(LEAVE_WHILE, node->line, node->pos, NULL));

    if (else_statement != NULL)
    {
        stack_push(tree_traversal, else_statement);
        generate_while_jump_to_else_if_false(node->line);
    }
    else
    {
        generate_while_jump_to_endwhile_if_false(node->line);
    }

    // leave scope
    stack_push(tree_traversal, ast_node_init(LEAVE_SCOPE, 0, 0, NULL));
    stack_push(tree_traversal, body);

    return generator_result(current_scope, ERROR_OK);
}

generator_result_t handle_while_else(scope_t current_scope, ast_t node, stack_t tree_traversal) {
    scope_t scope = new_scope(current_scope, node);

    stack_push(tree_traversal, ast_node_init(LEAVE_SCOPE, 0, 0, NULL));

    // add body for generate
    stack_push(tree_traversal, array_nodes_get(node->nodes, 0));

    // Get LEAVE_WHILE line
    generate_while_else_label(current_scope->root->line);
    return generator_result(scope, ERROR_OK);
}

generator_result_t handle_continue(scope_t current_scope) {
    scope_t while_scope = find_first_node_type_in_scope(current_scope, WHILE);
    generate_while_jump(while_scope->root->line);
    return generator_result(current_scope, ERROR_OK);
}

generator_result_t handle_break(scope_t current_scope) {
    scope_t while_scope = find_first_node_type_in_scope(current_scope, WHILE);
    generate_endwhile_jump(while_scope->root->line);
    return generator_result(current_scope, ERROR_OK);
}

generator_result_t handle_leave_scope(scope_t current_scope, ast_t node)
{
    ast_node_type_t parent_type = current_scope->parent_scope->root->node_type;
    ast_node_type_t current_type = current_scope->root->node_type;

    if (parent_type == LEAVE_IF)
    {
        generate_endif_jump(current_scope->parent_scope->root->line);
    }
    else if (current_type == FUNCTION_DEFINITION)
    {
        generate_function_footer();
    }
    else if (current_type == WHILE)
    {
        generate_while_jump(node->line);
    }
    else if (current_type == LEAVE_WHILE)
    {
        generate_endwhile(node->line);
    }

    return generator_result(delete_scope(current_scope), ERROR_OK);
}

generator_result_t handle_node(scope_t current_scope, ast_t node, stack_t tree_traversal)
{
    switch (node->node_type)
    {
    case LEAVE_SCOPE:
        // leave scope
        return handle_leave_scope(current_scope, node);
    case LEAVE_IF:
        // end if
        generate_endif(node->line);
        return generator_result(delete_scope(current_scope), ERROR_OK);
    case PASS:
        // do nothing
        return generator_result(current_scope, ERROR_OK);
    case EXPRESSION:
        // expression without assignment is totally uselss, dont generate it
        return generator_result(current_scope, ERROR_OK);
    case RETURN:
        return handle_return(current_scope, node);
    case CONTINUE:
        return handle_continue(current_scope);

    case BREAK:
        return handle_break(current_scope);

    case FUNCTION_DEFINITION:
        return handle_function_definition(current_scope, node, tree_traversal);

    case FUNCTION_CALL:
        return handle_function_call(current_scope, node);
    case ASSIGN:
        return handle_assignment(current_scope, node);

    case WHILE:
        return handle_while(current_scope, node, tree_traversal);
    case WHILE_ELSE:
        return handle_while_else(current_scope, node, tree_traversal);
    case IF:
        handle_conditions(current_scope, node);
        return handle_if(current_scope, node, tree_traversal);
    case ELIF:
    case ELSE:
        return handle_if_elif_else(current_scope, node, tree_traversal);

    case CONSEQUENT:
        return handle_consequent(current_scope, node, tree_traversal);

    default:
        fprintf(stderr, "Forgot to implement %d on line %u\n", node->node_type, node->line);
        return generator_result(current_scope, ERROR_INTERNAL);
    }
}

enum error_codes generate(ast_t ast)
{
    stack_t tree_traversal = stack_init();
    stack_push(tree_traversal, ast);

    scope_t scope = new_scope(NULL, ast);

    while (!stack_empty(tree_traversal))
    {
        ast_t node = stack_pop(tree_traversal);
        generator_result_t result = handle_node(scope, node, tree_traversal);
        scope = result.current_scope;
    }

    // we should bubble up to root
    assert(scope->root == ast);

    delete_scope(scope);

    stack_destroy(tree_traversal);
    ast_delete(ast);

    return ERROR_OK;
}
