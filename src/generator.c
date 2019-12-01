/**
 * IFJ2019 @ VUT FIT Brno
 * 09.11.2019
 *
 * Variant 2
 * Team: 054
 *
 * Authors:
 * Marian Lorinc (xlorin01)
 * Lukas Javorsky (xjavor20)
 * Patrik Ondriga (xondri08)
 * Peter Vinarcik (xvinar00)
 *
 * File: generator.c
 *
 */

#include "generator.h"
#include "parser_ast.c"
#include "array_nodes.h"

error_codes generate(ast_t ast, scope_t scope)
{
    switch (ast->node_type)
    {
        case PASS:
            return ERROR_OK;

        case RETURN:
            return ERROR_OK;

        case CONTINUE:
            return ERROR_OK;

        case BREAK:
            return ERROR_OK;

        case FUNCTION_DEFINITION:
            generate_function_definition(ast, scope)
            return ERROR_OK;

        case ASSIGN:
            return ERROR_OK;

        case WHILE:
            return ERROR_OK;

        case IF:
            return ERROR_OK;

        case ELIF:
            return ERROR_OK;

        case ELSE:
            return ERROR_OK;

        case NONE:
            return ERROR_OK;

        case EXPRESSION:
            return ERROR_OK;

        default:
            fprintf(stderr, "Forgot to implement %d\n", node->node_type);
            break;
    }

    return ERROR_OK;
}

void generate_start()
{
    printf(".IFJcode19\n");
    printf("JUMP $$main\n\n");
}

void generate_function_definition(ast_t ast, scope_t scope)
{
    size_t pom = get_number_of_parameters(ast);
    generate_label(ast);
    generate_define_frame_and_retvalue();
    generate_defvar_of_params(ast, pom);
    //TODO here will be body of function

    generate_end_of_function(ast);
}

size_t get_number_of_parameters(ast_t ast)
{
    ast_t parameters = array_nodes_get(ast, 0); //extract params from ast [0]
    size_t pom = array_nodes_size(parameters->nodes); //get num of params
    return pom;
}

void generate_label(ast_t ast)
{
    printf("LABEL $%s\n", ast->data);
}

void generate_defvar_of_params(ast_t ast, size_t numberOfParams)
{
    ast_t parameters = get_values_of_params(ast);

    for(size_t i = 0; i < numberOfParams; i++){
        ast_t param_name = array_nodes_get(parameters->nodes, i);
        printf("DEFVAR LF@%s\n", param_name->data)
        printf("MOVE LF@%s LF@%%d\n", param_name->data, i+1);
    }
}

void generate_define_frame_and_retvalue()
{
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@%retval\n");
    printf("MOVE LF@%retval nil@nil\n");
}

ast_t get_values_of_params(ast_t ast)
{
    ast_t parameters = array_nodes_get(ast->nodes, 0);

    for (size_t i = 0; i < array_nodes_size(parameters->nodes); i++){
        ast_t param_names = array_nodes_get(parameters->nodes, i);
    }

    return param_names;
}

void generate_end_of_function(ast_t ast, char* type)
{
    printf("MOVE LF@%retval LF@returnValue\n") //IN instruction returnValue
    printf("POPFRAME\n");                      //must be always saved return value
    printf("RETURN\n\n");                      //double newline for better look
}
