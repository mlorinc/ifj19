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

tToken_type last_token_type = TFIRSTINDENT;
stack_t stack;

int* p_int(int number)
{
    int* tmp = (int*) malloc(sizeof(int));
    if(tmp != NULL)
        *tmp = number;
    return tmp;
}

tToken indent_counter()
{
    tToken token;
    token.value = NULL;
    token.type = TNOTHING;
    int* p_number = NULL;

    if(last_token_type == TFIRSTINDENT)
    {
        stack = stack_init();
        if(stack == NULL)
            token.type = TERR;
        else
        {        
            p_number = p_int(0);
            if(p_number == NULL)
                token.type = TERR;
            else if(stack_push(stack, p_number) != 0)
            {
                token.type = TERR;
                free(p_number);
            }
            else
                token.type = TNOTHING;
        }
    }
    else if(last_token_type == TNEWLINE || last_token_type == TDEDENT)
    {
        int c;
        int counter = 0;
        int* number_on_top = stack_top(stack);
        while((c = getchar()) == ' ')
        {
            counter++;
        }
        ungetc(c, stdin);
        if(number_on_top < counter)
        {
            p_number = p_int(counter);
            if(p_number == NULL)
            {
                token.type = TERR;
                stack_destroy(stack);
            }
            else if(stack_push(stack, p_number) != 0)
            {
                token.type = TERR;
                free(p_number);
                stack_destroy(stack);
            }
            else
                token.type = TINDENT;
        }
        else if(number_on_top > counter)
        {
            stack_pop(stack);
            number_on_top = stack_top(stack);
            if(*number_on_top == 0 && counter != 0)
                token.type = TERR;
            else
            {
                token.type = TDEDENT;
            for(; counter != 0; counter--)
        }
    }
    return token;
}

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