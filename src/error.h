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
 * File: error.h
 *
 */

#ifndef IFJ19_ERROR_H
#define IFJ19_ERROR_H

enum error_codes
{
    ERROR_OK = 0,       //code compiled without any error
    ERROR_LEX = 1,      //lexical error
    ERROR_SYNTAX = 2,   //syntax error
    ERROR_SEM = 3,      //semantic error -> Undefined function/variable, redefinition of function or variable
    ERROR_SEM_RUN = 4,  //semantic error -> during running, error in arithmetic/string literals
    ERROR_SEM_PRG = 5,  //semantic error -> bad number of params in function call
    ERROR_SEM_OTH = 6,  //semantic error -> other semantic errors
    ERROR_ZERO_DIV = 9, //dividing by zero
    ERROR_INTERNAL = 99 //compiler error (not affected by input) example: error during memory allocation
};


/**
 * Error handling function
 */
void error_exit(int err_number);

#endif //IFJ19_ERROR_H
