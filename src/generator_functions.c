#include "queue.h"
#include "generator_functions.h"
#include "scaner.h"
#include "string_convertor.h"
#include "array_nodes.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

bool is_literal(tToken *token)
{
    return token->type == TINT || token->type == TFLOAT || token->type == TSTRING;
}

void generate_expression(scope_t scope, queue_t expression)
{
    while (!queue_empty(expression))
    {
        tToken *token = queue_pop(expression);
        if (is_literal(token))
        {
            char *buffer = ptr_string_c_string(token->value);
            if (token->type == TFLOAT)
            {
                printf("PUSHS float@%a\n", atof(buffer));
            }
            else
            {
                printf("PUSHS %s@%s\n", token->type == TINT ? "int" : "string", buffer);
            }
            free(buffer);
        }

        else if (token->type == TIDENTIFICATOR)
        {
            char *buffer = ptr_string_c_string(token->value);
            scope_t variable_scope = find_scope_with_defined_variable(scope, token->value);
            if (variable_scope != NULL)
            {
                printf("PUSHS LF@%s\n", buffer);
            }
            else
            {
                printf("PUSHS GF@%s\n", buffer);
            }
            free(buffer);
        }
        else
        {
            switch (token->type)
            {
            case TADD:
                printf("ADDS\n");
                break;
            case TSUB:
                printf("SUBS\n");
                break;
            case TMUL:
                printf("MULS\n");
                break;
            case TDIV:
                printf("SUBS\n");
                break;
            case TGT:
                printf("GTS\n");
                break;
            case TGTE:
                printf("POPS GF@TypeCheck1\n");
                printf("POPS GF@TypeCheck2\n");
                printf("PUSHS GF@TypeCheck2\n");
                printf("PUSHS GF@TypeCheck1\n");
                printf("GTS\n");
                printf("PUSHS GF@TypeCheck2\n");
                printf("PUSHS GF@TypeCheck1\n");
                printf("EQS\n");
                printf("ORS\n"); //ak je to eq || gt tak to bude true, ak nie, false
                break;
            case TLT:
                printf("LTS\n");
                break;
            case TLTE:
                printf("POPS GF@TypeCheck1\n");
                printf("POPS GF@TypeCheck2\n");
                printf("PUSHS GF@TypeCheck2\n");
                printf("PUSHS GF@TypeCheck1\n");
                printf("LTS\n");
                printf("PUSHS GF@TypeCheck2\n");
                printf("PUSHS GF@TypeCheck1\n");
                printf("EQS\n");
                printf("ORS\n"); //ak je to eq || gt tak to bude true, ak nie, false
                break;
            case TEQ:
                printf("EQS\n");
                break;
            case TNE:
            default:
                fprintf(stderr, "%s:%d: Unimplemented token type %d\n", __FILE__, __LINE__, token->type);
                break;
            }
        }
    }
}

void retype_int_to_float(size_t line)
{
    (void)line;
}

/**
 * Generates ifj19 code for if/elif condition
 * @param expression condition. If label name is "else", expression is set to NULL
 * @param label_name name of label "if" | "elif" | "else" | "ENDIF (if else is missing)"
 * @param line line of if or elif
 */
void generate_condition(scope_t scope, queue_t expression, char *label_name, size_t line)
{
    if (expression == NULL)
    {
        printf("JUMP %s$%zu\n", label_name, line);
    }
    else
    {
        generate_expression(scope, expression);
        printf("PUSHS bool@true\n");
        printf("JUMPIFEQS %s$%zu\n", label_name, line);
    }
}

/**
 * Generates ENDIF label for if elif else sequence
 * LABEL ENDIF$line
 * @param line
 */
void generate_endif(size_t line)
{
    printf("LABEL ENDIF$%zu\n", line);
}

/**
 * Generates ENDIF jump for if elif else. It is used as last command to get out of if conditions.
 * jmp ENDIF$line
 * @param line
 */
void generate_endif_jump(size_t line)
{
    printf("JUMP ENDIF$%zu\n", line);
}

/**
 * Generates return. If expression is NULL, it means it returns None
 */
void generate_return(scope_t scope, queue_t expression)
{
    if (expression == NULL)
    {
        printf("MOVE LF@%%retval nil@nil\n");
        printf("POPFRAME\n");
        printf("RETURN\n\n");
        return;
    }

    generate_expression(scope, expression);

    printf("POPS LF@%%retval\n");
    printf("POPFRAME\n"); //must be always saved return value
    printf("RETURN\n\n"); //double newline for better look after function definition
}

/**
 * Generates header of function. params is never NULL.
 */
void generate_function_header(char *fun_name, array_nodes_t params)
{
    printf("LABEL %s\n", fun_name);       //LABEL fun_name
    printf("PUSHFRAME\n");                //PUSHFRAME
    printf("DEFVAR LF@%%retval\n");       //DEFVAR LF@%retval
    printf("MOVE LF@%%retval nil@nil\n"); //DEFVAR LF@%retval nil@nil
    printf("DEFVAR LF@TypeCheck1\n");
    printf("DEFVAR LF@TypeCheck2\n");
    for (size_t i = 0; i < array_nodes_size(params); i++)
    {                                                  //DEFVAR LF@parami
        ast_t param_name = array_nodes_get(params, i); //MOVE LF@parami LF@%i+1
        char *buffer = ptr_string_c_string(param_name->data);
        printf("DEFVAR LF@%s\n", buffer); //after this generate function body
        printf("MOVE LF@%s LF@%%%zu\n", buffer, i + 1);
        free(buffer);
    }
}

/**
 * Generates code before function returns
 */
void generate_function_footer()
{
    printf("POPFRAME\n");
    printf("RETURN\n");
    //we dont have to move any return value into @%retval despite of fact, that we declarate it... of course in case,
    //when we dont want to return anything from function
    //f.e.: def foo(a,b):
    //          print(a,b)
    //          return
}

bool is_local_frame(scope_t scope, ptr_string_t id)
{
    scope_t return_scope = find_scope_with_defined_variable(scope, id);
    return return_scope != NULL;
}

/**
 * Generated code which calls function
 * @param node function call node
 * @param current_scope scope, where call is performed. Use it do deduce if it should use local or global variable
 */
void generate_function_call(scope_t current_scope, ast_t func_call)
{
    printf("CREATEFRAME\n");

    ast_t params = array_nodes_get(func_call->nodes, 0);

    for (size_t i = 0; i < array_nodes_size(params->nodes); i++)
    {
        ast_t param = array_nodes_get(params->nodes, i);
        char *buffer = ptr_string_c_string(param->data);

        printf("DEFVAR TF@%%%zu\n", i + 1);

        char *temp;
        switch (param->node_type)
        {
        case STRING_LITERAL:
            temp = ptr_string_c_string(ifj_string_to_code_string(param->data));
            printf("MOVE TF@%%%zu string@%%%s\n", i + 1, temp);
            free(temp);
            break;
        case FLOAT_LITERAL:
            printf("MOVE TF@%%%zu float@%%%a\n", i + 1, atof(buffer));
            break;
        case INT_LITERAL:
            printf("MOVE TF@%%%zu int@%%%s\n", i + 1, buffer);
            break;
        case ID:
            temp = ptr_string_c_string(param->data);
            if (is_local_frame(current_scope, param->data))
            {
                printf("MOVE TF@%%%zu LF@%%%s\n", i + 1, temp);
            }
            else
            {
                printf("MOVE TF@%%%zu GF@%%%s\n", i + 1, temp);
            }
            free(temp);
            break;
        default:
            fprintf(stderr, "%s:%d: Did not match any node type (%d)\n", __FILE__, __LINE__, param->node_type);
        }

        free(buffer);
    }

    char *fn_name = ptr_string_c_string(func_call->data);
    printf("CALL %s\n", fn_name);
    free(fn_name);
}

/**
 * Generates code which stores return value of function to id
 */
void generate_function_call_assignment(scope_t current_scope, const char *id, ast_t func_call)
{
    ast_t params = array_nodes_get(func_call->nodes, 0);
    char *fn_name = ptr_string_c_string(func_call->data);

    if (strcmp(fn_name, "print") == 0)
    {
        generate_print(current_scope, params->nodes);
        if (is_local_frame(current_scope, func_call->data))
        {
            printf("MOVE LF@%s nil@nil\n", id);
        }
        else
        {
            printf("MOVE LF@%s nil@nil\n", id);
        }
        free(fn_name);
        return;
    }

    for (size_t i = 0; i < array_nodes_size(params->nodes); i++)
    {
        ast_t param = array_nodes_get(params->nodes, i);
        char *buffer = ptr_string_c_string(param->data);

        printf("DEFVAR TF@%%%zu\n", i + 1);

        char *temp;
        switch (param->node_type)
        {
        case STRING_LITERAL:
            temp = ptr_string_c_string(ifj_string_to_code_string(param->data));
            printf("MOVE TF@%%%zu string@%%%s\n", i + 1, temp);
            free(temp);
            break;
        case FLOAT_LITERAL:
            printf("MOVE TF@%%%zu float@%%%a\n", i + 1, atof(buffer));
            break;
        case INT_LITERAL:
            printf("MOVE TF@%%%zu int@%%%s\n", i + 1, buffer);
            break;
        case ID:
            temp = ptr_string_c_string(param->data);
            if (is_local_frame(current_scope, param->data))
            {
                printf("MOVE TF@%%%zu LF@%%%s\n", i + 1, temp);
            }
            else
            {
                printf("MOVE TF@%%%zu GF@%%%s\n", i + 1, temp);
            }
            free(temp);
            break;
        default:
            fprintf(stderr, "%s:%d: Did not match any node type (%d)\n", __FILE__, __LINE__, param->node_type);
        }

        free(buffer);
    }

    printf("CALL %s\n", fn_name);
    free(fn_name);

    if (is_local_frame(current_scope, func_call->data))
    {
        printf("POPS LF@%s\n", id);
    }
    else
    {
        printf("POPS GF@%s\n", id);
    }
}

/**
 * Generates code which stores expression value to id
 */
void generate_expression_assignment(scope_t current_scope, const char *id, ast_t expression)
{
    generate_expression(current_scope, expression->data);

    ptr_string_t str = ptr_string(id);
    if (is_local_frame(current_scope, str))
    {
        printf("POPS LF@%s\n", id);
    }
    else
    {
        printf("POPS GF@%s\n", id);
    }
    ptr_string_delete(str);
}

/**
 * Generates while label
 * WHILE$line
 */
void generate_while_label(size_t line)
{
    printf("LABEL WHILE$%zu\n", line);
}

/**
 * Generate while condition
 */
void generate_while_condition(scope_t scope, queue_t expression)
{
    generate_expression(scope, expression);
}

/**
 * Generate WHILEELSE$line label
 */
void generate_while_else_label(size_t line)
{
    printf("LABEL WHILEELSE$%zu\n", line);
}

/**
 * Generate jump to else structure if condition was false
 */
void generate_while_jump_to_else_if_false(size_t line)
{
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS WHILEELSE$%zu\n", line);
}

/**
 * Jump out of while if condition is false
 */
void generate_while_jump_to_endwhile_if_false(size_t line)
{
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS ENDWHILE$%zu\n", line);
}

/**
 * Generate jump to WHILE$line
 */
void generate_while_jump(size_t line)
{
    printf("JUMP WHILE$%zu\n", line);
}

/**
 * Generate jump to ENDWHILE$line
 */
void generate_endwhile_jump(size_t line)
{
    printf("JUMP ENDWHILE$%zu\n", line);
}

/**
 * Generates end while label
 * ENDWHILE$line
 */
void generate_endwhile(size_t line)
{
    printf("LABEL ENDWHILE$%zu\n", line);
}

void generate_print(scope_t scope, array_nodes_t params)
{
    size_t size = array_nodes_size(params);
    for (size_t i = 0; i < size; i++) // Until all params
    {
        ast_t param = array_nodes_get(params, i);
        char *buffer = ptr_string_c_string(param->data);

        char *frame_modifier;
        switch (param->node_type)
        {
        case STRING_LITERAL:
            printf("WRITE string@%s\n", buffer);
            break;
        case INT_LITERAL:
            printf("WRITE int@%s\n", buffer);
            break;
        case FLOAT_LITERAL:
            // todo: i am not sure, fix it
            printf("WRITE float@%a\n", atof(buffer));
            break;
        case ID:
            frame_modifier = find_scope_with_defined_variable(scope, param->data) != NULL ? "LF" : "GF";
            printf("WRITE %s@%s\n", frame_modifier, buffer); // Write the param
            break;
        default:
            break;
        }

        if (i + 1 == size) // Last term
        {
            printf("WRITE string@\\010\n"); // Last term must have newline after
        }
        else
        {
            printf("WRITE string@\\032\n"); // After every term must be space
        }
        free(buffer);
    }
}
