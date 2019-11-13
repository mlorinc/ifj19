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
 * File: scaner.c
 *
 */

#include "scaner.h"
#include "error.h"
#include <stdio.h>
#include "stack.h"

get_token(){

    tToken = token;
    char c; // New char on input
    t_stack stack = stack_init();

    ptr_string_t string = NULL; // Space for something readed from input

    while ((c=getchar()) != EOF)    // Until whole input is readed
    {
        
    }
    

}