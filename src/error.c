/**
 * IFJ2019 @ VUT FIT Brno
 * 13.10.2019
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
 * File: error.c
 *
 */

#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void error_exit(int err_number)
{
    if(err_number == ERROR_OK)
    {
        exit(ERROR_OK);
    }
    else if (err_number == ERROR_LEX)
    {
        fprintf(stderr,"ERROR: Lexical error!\n");
        exit(ERROR_LEX);
    }
    else if (err_number == ERROR_SYNTAX)
    {
        fprintf(stderr,"ERROR: Syntax error!\n");
        exit(ERROR_SYNTAX);
    }
    else if (err_number == ERROR_SEM)
    {
        fprintf(stderr,"ERROR: Semantics error -> Undefined function or variable!\n");
        exit(ERROR_SEM);
    }
    else if (err_number == ERROR_SEM_RUN)
    {
        fprintf(stderr,"ERROR: Semantics error -> Arithmetic / String Literal!\n");
        exit(ERROR_SEM_RUN);
    }
    else if (err_number == ERROR_SEM_PRG)
    {
        fprintf(stderr,"ERROR: Semantics error -> Wrong number of params in called function!\n");
        exit(ERROR_SEM_PRG);
    }
    else if (err_number == ERROR_SEM_OTH)
    {
        fprintf(stderr,"ERROR: Semantics error!\n");
        exit(ERROR_SEM_OTH);
    }
    else if (err_number == ERROR_ZERO_DIV)
    {
        fprintf(stderr,"ERROR: Dividing by zero!\n");
        exit(ERROR_ZERO_DIV);
    }
    else if (err_number == ERROR_INTERNAL)
    {
        fprintf(stderr,"ERROR: Internal error -> cannot allocate memory!\n");
        exit(ERROR_INTERNAL);
    }
}