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
#include <ctype.h>

tToken_type last_token_type = TFIRSTINDENT;
stack_t stack;

/* These two numbers are for better error message */
unsigned int row = 0;   // What row are we at
unsigned int character_position = 0;

int* p_int(int number)
{
    int* tmp = (int*) malloc(sizeof(int));
    if(tmp != NULL)
        *tmp = number;
    return tmp;
}

tToken_type push_indent_on_stack(int indent_number)
{
    int* p_number = NULL;
    p_number = p_int(indent_counter);
    if(p_number == NULL)    //check for new malloc pointer to int
    {
        stack_destroy(stack);
        return TERR;
    }
    else if(stack_push(stack, p_number) != 0)   //check if stack push was unsuccessfully
    {
        free(p_number);
        stack_destroy(stack);
        return TERR;
    }
    return TNOTHING;
}

tToken_type init_indent_counter()
{
    stack = stack_init();
    if(stack == NULL)   //check for stack init
        return TERR;
    else
        return push_indent_on_stack(0);
}

tToken indent_counter()
{
    tToken token;
    token.value = NULL;
    token.type = TNOTHING;

    if(last_token_type == TFIRSTINDENT) //initial counter for indent
    {
        token.type = init_indent_counter();
        if(token.type == TERR) //if initialization of counter was unsuccessfully
            return token;
    }
    if(last_token_type == TNEWLINE || last_token_type == TDEDENT || last_token_type == TFIRSTINDENT)
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
            token.type = push_indent_on_stack(counter);
            if(token.type == TERR)  //check if push on stack was unsuccessfully
                return token;
            else
                token.type = TINDENT;
        }
        else if(number_on_top > counter)
        {
            stack_pop(stack);
            number_on_top = stack_top(stack);
            if(*number_on_top < counter )
                token.type = TERR;
            else
                token.type = TDEDENT;
            for(; counter != 0; counter--)
                ungetc(' ', stdin);
        }
        else
            token.type = TNOTHING;
    }
    return token;
}
/**
 * Process all states from starting state of automata
 * @param c Stdin char
 * @param state Automata state
 * @param string Temporary string that will be sent as token's contet
 */
void start_state(char c, tState* state, ptr_string_t string)
{
    c = tolower(c);
    if(c >= '1' && c <= '9')
        *state = sInteger;
    else if(c == '0')
        *state = sInteger0;
    else if(c == '\'')
        *state = sStringStart;
    else if(c == '"')
        *state = sBlockCommentStart1;
    else if(c == '#')
        *state = sLineComment;
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
        ptr_string_delete_last(string); // Delete char ' ' from token value
    }
    else if (c == '\n')
    {
        *state = sNewLine;
        ptr_string_delete_last(string); // Delete char '\n' from token value
    }
    else    // Undifiend char
        *state = sLexErr;
}

/**
 * Put the last char back to stdin and fill the token value and type, 
 * also set the last_token_type.
 * @param token_ptr Token pointer
 * @param string String in token's value
 * @param c Char that will be put back to stdin
 * @param token_type Token's type
 */
void token_fill(tToken *token_ptr, ptr_string_t string, char c, tToken_type token_type)
{
    ptr_string_delete_last(string);
    putchar(c); // Put the last char back to stdout
    token_ptr->value = string;
    last_token_type = token_ptr->type = token_type;
}

tToken get_token()
{
    tToken token;
    int c; // New char on input
    tState state = sStart;
    ptr_string_t string = NULL; // Space for something readed from input

    while ((c=getchar()) != EOF)    // Until whole input is readed
    {
        ptr_string_append(&string, c); //TODO check if append was correct
        character_position++;   // We've moved by one character

        switch(state)
        {
            /*************************** Start ********************************/   
            case sStart:
                start((char) c, &state, string);
                break;

            case sNewLine:
                row++;  // We have moved one row
                token_fill(&token, string, c, TNEWLINE);
                token.value = NULL; // Fix the value to NULL, because it's not needed
                return token;
                break;

            /*************************** Numbers ********************************/
            case sInteger0:
                if (c=getchar() == '.')
                    state = sFloatDot;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    return token;
                }
                break;
            case sInteger:
                if (c >= '0' && c <= '9')
                    state = sInteger;
                else if (c == '.')
                    state = sFloat;
                else if (c == 'e' || c == 'E')
                    state = sExponent;
                else    // End of the int number
                {
                    token_fill(&token, string, c, TINT);
                    return token;
                }
                break;
            case sFloat:
                if (c >= '0' && c <= '9')
                    state = sFloat;
                else if (c == 'e' || c == 'E')
                    state = sExponent;
                else    // End of the float number
                {
                    token_fill(&token, string, c, TFLOAT);
                    return token;
                }
                break;
            case sExponent:
                if (c >= '0' && c <= '9')
                    state = sEndExpondent;
                else if (c == '-' || c == '+')
                    state = sExponentOperator;
                else    // Error (not ending state)
                {
                    token_fill(&token, string, c, TLEXERR);
                    return token;
                }
                break;
            case sExponentOperator:
                if (c >= '0' && c <= '9')
                    state = sFloat;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    return token;
                }

            /*************************** Comments ********************************/
            case sLineComment:
                if (c == '\n')  // End of comment
                {
                    *state = sStart;
                    putchar(c); // Put the '/n' back to stdin
                }
                else
                    *state = sLineComment;
                break;
            case sBlockCommentStart1:
                if (c == '"')   // It means now you have second '""'
                    *state = sBlockCommentStart2;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    return token;
                }
                break;
            case sBlockCommentStart2:
                if (c == '"')   // It means now you have third '"""' and the comment has started
                    *state = sBlockComment;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    return token;
                }
                break;
            case sBlockComment:
            // TODO add if for '\n' to increase row global variable;
                if (c == '"')   // It means now you have first '"'
                    *state = sBlockCommentEnd1;
                else    // Everything inside the block comment
                    *state = sBlockComment;
                break;
            case sBlockCommentEnd1:
                if (c == '"')   // It means now you have second '""'
                    *state = sBlockCommentEnd2;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    return token;
                }
                break;
            case sBlockCommentEnd2:
                if (c == '"')   // It means now you have third '"""' and the comment has ended
                    *state = sStart;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    return token;
                }
                break;    
            
            /************************* Indetificator/Keyword *****************************/
            case sIdentificatorOrKeyWord:

                
        }
    }
    

}