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

void inputs()
{
    printf(
        "LABEL inputs\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "READ LF@%retval string\n"
        "POPFRAME\n"
        "RETURN\n\n"
        );
}

void inputi()
{
    printf(
        "LABEL inputi\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "READ LF@%retval int\n"
        "POPFRAME\n"
        "RETURN\n\n"
        );
}

void inputf()
{
    printf(
        "LABEL inputf\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "READ LF@%retval float\n"
        "POPFRAME\n"
        "RETURN\n\n"
        );
}

void len()
{
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

void substr()
{
    printf(
        "LABEL substr\n"
        "PUSHFRAME\n"
        "DEFVAR LF@%retval\n"
        "MOVE LF@%retval nil@nil\n"
        "DEFVAR LF@s\n"
        "MOVE LF@s LF@%1\n"
        "DEFVAR LF@i\n"
        "MOVE LF@i LF@%2\n"
        "DEFVAR LF@n\n"
        "MOVE LF@n LF@%3\n"
        "DEFVAR LF@jump\n"
        "DEFVAR LF@lenstring\n"
        "STRLEN LF@lenstring LF@s\n"
        "LT LF@jump LF@i int@0\n"
        "JUMPIFEQ badi LF@jump bool@true\n"
        "GT LF@jump LF@lenstring LF@i\n"
        "JUMPIFNEQ badi LF@jump bool@true\n"
        "LT LF@jump LF@n int@0\n"
        "JUMPIFEQ badi LF@jump bool@true\n"
        "DEFVAR LF@lenstringmini\n"
        "SUB LF@lenstringmini LF@lenstring LF@i\n"
        "GT LF@jump LF@n LF@lenstringmini\n"
        "JUMPIFEQ lenlessn LF@jump bool@true\n"
        "JUMP okn\n"
        "LABEL lenlessn\n"
        "MOVE LF@n LF@lenstringmini\n"
        "LABEL okn\n"
        "DEFVAR LF@char\n"
        "DEFVAR LF@tmp\n"
        "EQ LF@jump LF@n int@0\n"
        "JUMPIFEQ substrend LF@jump bool@true\n"
        "GETCHAR LF@char LF@s LF@i\n"
        "MOVE LF@%retval LF@char\n"
        "ADD LF@tmp LF@i int@1\n"
        "MOVE LF@i LF@tmp\n"
        "SUB LF@tmp LF@n int@1\n"
        "MOVE LF@n LF@tmp\n"
        "EQ LF@jump LF@n int@0\n"
        "JUMPIFEQ substrend LF@jump bool@true\n"
        "LABEL substrwhile\n"
        "GETCHAR LF@char LF@s LF@i\n"
        "CONCAT LF@tmp LF@%retval LF@char\n"
        "MOVE LF@%retval LF@tmp\n"
        "ADD LF@tmp LF@i int@1\n"
        "MOVE LF@i LF@tmp\n"
        "SUB LF@tmp LF@n int@1\n"
        "MOVE LF@n LF@tmp\n"
        "EQ LF@jump LF@n int@0\n"
        "JUMPIFNEQ substrwhile LF@jump bool@true\n"
        "JUMP substrend\n"
        "LABEL badi\n"
        "MOVE LF@%retval nil@nil\n"
        "LABEL substrend\n"
        "POPFRAME\n"
        "RETURN\n"
        )
}
