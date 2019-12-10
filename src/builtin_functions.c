/**
 * IFJ2019 @ VUT FIT Brno
 * 10.12.2019
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
 * File: builtin_functions.c
 *
 */

#include <stdio.h>
#include "builtin_functions.h"
#include "array_nodes.h"

void builtin_function()
{
    printf(
        "LABEL inputs\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "READ LF@%retval string\n"
        "POPFRAME\n"
        "RETURN\n\n"
        );
    printf(
        "LABEL inputi\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "READ LF@%retval int\n"
        "POPFRAME\n"
        "RETURN\n\n"
        );
    printf(
        "LABEL inputf\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "READ LF@%retval float\n"
        "POPFRAME\n"
        "RETURN\n\n"
        );
    
}

void print_builtin_function(array_nodes_t params){

    printf(
        "LABEL print\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "MOVE LF@%retval nil@nil\n"
    );
    for(size_t i = 0; i < array_nodes_size(params); i++)    // Until all params
    { 
        ast_t param_name = array_nodes_get(params, i);
        char *buffer = ptr_string_c_string(param_name->data);
        printf("WRITE LF@%s\n", buffer);    // Write the param
        if (i+1 == array_nodes_size(params))    // Last term
        {
            printf("WRITE string@\\010");    // Last term must have newline after
        }
        else
        {
            printf("WRITE string@\\032");    // After every term must be space
        }
        printf("MOVE LF@%s LF@%%%zu\n", buffer, i+1);
        free(buffer);
    }

    printf(
        "POPFRAME\n"
        "RETURN\n\n")
}

