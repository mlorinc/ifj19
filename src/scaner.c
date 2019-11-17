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
stack_t stack;  //stack for indent numbers

/* These two numbers are for better error message */
unsigned int row = 0;   // What row are we at
unsigned int character_position = 0;

/* Global array of possible keywords */
char *keywords[7] = {"def", "else", "if", "None", "pass", "return", "while"};

/**
 * Function for malloc pointer to int
 * @param number number whitch will be in pointer
 * @return pointer to int with value number
 */
int* p_int(int number)
{
    int* tmp = (int*) malloc(sizeof(int));
    if(tmp != NULL)
        *tmp = number;
    return tmp;
}

/**
 * push number on stack and check if it was successfully
 * @param indent_number integer whitch will be push
 * @return TNOTHING if push was successfully else TERR
 */
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

/**
 * initialization stack and push on top 0
 * @return TNOTHING if init was successfully else TERR
 */
tToken_type init_indent_counter()
{
    stack = stack_init();
    if(stack == NULL)   //check for stack init
        return TERR;
    else
        return push_indent_on_stack(0);
}

/**
 * Indent counter
 * @return token with one of this token types:
 *      TERR -> stack or malloc error
 *      TNOTHING -> if indent is the same as before
 *      TLEXERR -> indent error
 */
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
        character_position = counter;
        ungetc(c, stdin);
        if(c == '#' || c == '"')    //check for comment after indent
            return token;

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
            if(*number_on_top < counter)
            {
                token.type = TLEXERR;
                fprintf(stderr, "Indent error on line %d.", row);
            }
            else
                token.type = TDEDENT;
            for(; counter != 0; counter--)  //this is for case, there will be more detent
                ungetc(' ', stdin);
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

/**
 * Append c to str
 * @param c char whitch will be append
 * @param str string where I want append
 * @return str + c
 */
ptr_string_t char_append(char c, ptr_string_t str)
{
    ptr_string_t newStr = NULL;
    if((newStr = ptr_string_append(str, c)) == NULL)
    {
        newStr = ptr_string_new_with_length(1);
        newStr = ptr_string_append(str, c);
    }
    ptr_string_delete(str);
    return newStr;
}

tToken get_token()
{
    tToken token;
    int c; // New char on input
    tState state = sStart;
    ptr_string_t string = NULL; // Space for something readed from input

    token = indent_counter();
    last_token_type = token.type;
    if(token.type != TNOTHING) //if indent wasn't the same as before
        return token;

    while ((c=getchar()) != EOF)    // Until whole input is readed
    {
        if((string = char_append(c, string)) == NULL) //check if append was successfully
        {
            last_token_type = token.type = TERR;
            token.value = NULL;
            return token;
        }
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