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

/* Global array of possible keywords */
char *keywords[7] = {"def", "else", "if", "None", "pass", "return", "while"};

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
    if (token_type == TLEXERR)
    {
        token_ptr->value = NULL
    }
    else
    {
        token_ptr->value = string;
    }
    last_token_type = token_ptr->type = token_type;
}

/**
 * Prints the error message to stderr
 * @param message Error message
 * @param line Line in code
 * @param position What is the position of error
 */
void error_print(char *message, unsigned int line, unsigned int position)
{
    fprintf(stderr, "%s\n Error on line: %d.\n On position: %d.\n", message, line, position);
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
                if (c == '.')
                    state = sFloat;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    error_print("Char 'E' must be followed by '.'", row, character_position);
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
                    error_print("Char 'E' as exponent must be followed by '- or +' or a number", row, character_position);
                    return token;
                }
                break;
            case sExponentOperator:
                if (c >= '0' && c <= '9')
                    state = sFloat;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    error_print("Exponent must be followed by number", row, character_position);
                    return token;
                }

            /*************************** Comments ********************************/
            case sLineComment:
                if (c == '\n')  // End of comment
                {
                    state = sStart;
                    putchar(c); // Put the '/n' back to stdin
                }
                else
                    state = sLineComment;
                break;
            case sBlockCommentStart1:
                if (c == '"')   // It means now you have second '""'
                    state = sBlockCommentStart2;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    error_print("Block comment is not valid", row, character_position);
                    return token;
                }
                break;
            case sBlockCommentStart2:
                if (c == '"')   // It means now you have third '"""' and the comment has started
                    state = sBlockComment;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    error_print("Block comment is not valid", row, character_position);
                    return token;
                }
                break;
            case sBlockComment:
                if (c == '\n')  // We have moved by one line
                {
                    row++;
                    character_position = 0; // New line so possition is 0
                }
                if (c == '"')   // It means now you have first '"'
                    state = sBlockCommentEnd1;
                else    // Everything inside the block comment
                    state = sBlockComment;
                break;
            case sBlockCommentEnd1:
                if (c == '"')   // It means now you have second '""'
                    state = sBlockCommentEnd2;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    error_print("Block comment is not valid", row, character_position);
                    return token;
                }
                break;
            case sBlockCommentEnd2:
                if (c == '"')   // It means now you have third '"""' and the comment has ended
                    state = sStart;
                else
                {
                    token_fill(&token, string, c, TLEXERR);
                    error_print("Block comment is not valid", row, character_position);
                    return token;
                }
                break;    
            
            /************************* Indetificator/Keyword *****************************/
            case sIdentificatorOrKeyWord:
                // TODO after read is done, compare the string with array of keywords
                
        }
    }
    

}