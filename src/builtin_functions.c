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
    printf(
        "LABEL len\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "DEFVAR LF@param1\n"
        "MOVE LF@param1 LF@%1\n"
        "STRLEN LF@%retval LF@param1\n"
        "POPFRAME\n"
        "RETURN\n\n"
        );
}

