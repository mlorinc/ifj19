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
#include "ptr_string.h"
#include <ctype.h>

void start(char c, tState* state, ptr_string_t string)
{
    c = tolower(c);
    if(c >= 49 && c <= 57) // interval <1...9>
        *state = sInteger;
    else if(c == '0')
        *state = sInteger0;
    else if(c == '.')
        *state = sFloatDot;
    else if(c == '\'')
        *state = sStringStart;
    else if(c == '"')
        *state = sBlockCommentStart1;
    else if(c == '#')
        *state = sBlockComment;
    else if(c == '+')
        *state = sAdd;
    else if(c == '-')
        *state = sSub;
    else if(c == '*')
        *state = sMul;
    else if(c == '/')
        *state = sDiv;
    else if(c == '%')
        *state =sMod;
    else if(c == '<')
        *state = sLt;
    else if(c == '>')
        *state = sGt;
    else if(c == '=')
        *state = sAssign;
    else if(c == '!')
        *state = sExclMark;
    else if(c == '_' || c >= 97 && c<= 122)
        *state = sIdentificatorOrKeyWord;
    else if(c == ' ')
    {
        *state = sStart;
        ptr_string_delete_last(string); //delet char ' ' from token string
    }
}


tToken get_token(){

    tToken token;
    int c; // New char on input
    stack_t stack = stack_init();
    tState state = sStart;
    ptr_string_t string = NULL; // Space for something readed from input

    while ((c=getchar()) != EOF)    // Until whole input is readed
    {
        ptr_string_append(&string, c); //TODO check if append was correct

        switch(state)
        {
            case sStart:
                start((char) c, &state, string);
                break;
        }
    }
    

}