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
#include <string.h>

#define KEYWORDS_COUNT 7

tToken_type last_token_type = TFIRSTINDENT;
stack_t stack;  //stack for indent numbers

/* These two numbers are for better error message */
unsigned int row = 1;   // What row are we at
unsigned int character_position = 1;

/* Global array of possible keywords */
char *keywords[KEYWORDS_COUNT] = {"def", "else", "if", "None", "pass", "return", "while"};

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
    p_number = p_int(indent_number);
    if(p_number == NULL)    //check for new malloc pointer to int
    {
        stack_destroy(stack);
        return TERR;
    }
    else if(!stack_push(stack, p_number))   //check if stack push was unsuccessful
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
        if(token.type == TERR) //if initialization of counter was unsuccessful
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
        if(c == '#' || c == '"' || c == '\n')    //check for comment after indent or EOL
            return token;

        if(*number_on_top < counter)
        {
            token.type = push_indent_on_stack(counter);
            if(token.type == TERR)  //check if push on stack was unsuccessful
                return token;
            else
                token.type = TINDENT;
        }
        else if(*number_on_top > counter)
        {
            free(number_on_top);
            stack_pop(stack);
            number_on_top = stack_top(stack);
            if(*number_on_top < counter)
            {
                token.type = TLEXERR;
                fprintf(stderr, "Indent error on line %d.\n", row);
            }
            else
                token.type = TDEDENT;
            for(; counter != 0; counter--)  //this is for case, there will be more dedent
                ungetc(' ', stdin);
        }
    }
    return token;
}

/**
 * Put the last char back to stdin and fill the token value and type, 
 * also set the last_token_type.
 * @param token_ptr Token pointer
 * @param string String in token's value
 * @param c Char that will be put back to stdin
 * @param token_type Token's type
 */
tToken token_fill(tToken *token_ptr, ptr_string_t string, char c, tToken_type token_type)
{
    if(token_type != TIDENTIFICATOR && token_type != TKEYWORD && token_type != TBLOCKCOMMENTORLITERAL && (token_type < TFLOORDIV || token_type > TNE))
    {
        ptr_string_delete_last(string);
    }
    if (c != -1)
    {
        character_position--;
        ungetc(c, stdin); // Put the last char back to stdin
    }
    if (token_type == TLEXERR || token_type == TNEWLINE)
    {
        token_ptr->value = NULL;
        ptr_string_delete(string);
    }
    else
    {
        token_ptr->value = string;
    }
    last_token_type = token_ptr->type = token_type;
    return *token_ptr;
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
 * Process all states in starting state of automata
 * @param c Stdin char
 * @param state Automata state
 * @param string Temporary string that will be sent as token's contet
 */
void start_state(char c, tState* state, ptr_string_t string, tToken *token)
{
    c = tolower(c);
    if(c >= '1' && c <= '9')
        *state = sInteger;
    else if(c == '0')
        *state = sInteger0;
    else if(c == '\'')
    {
        *state = sString;
        ptr_string_delete_last(string);
    }
    else if(c == '"')
    {
        ptr_string_delete_last(string); // Delete first " from string
        *state = sBlockCommentStart1;
    }
    else if(c == '#')
        *state = sLineComment;
    else if(c == '+')
        token_fill(token, string, (int) -1, TADD);
    else if(c == '-')
        token_fill(token, string, (int) -1, TSUB);
    else if(c == '*')
        token_fill(token, string, (int) -1, TMUL);
    else if(c == ':')
        token_fill(token, string, (int) -1, TCOLON);
    else if(c == '/')
        *state = sDivOrFloorDiv;
    else if(c == '<')
        *state = sLtOrLte;
    else if(c == '>')
        *state = sGtOrGte;
    else if(c == '=')
        *state = sAssignOrEqual;
    else if(c == '!')
        *state = sExclMark;
    else if(c == '_' || (c >= 'a' && c<= 'z'))
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
    else if (c == '(')
        token_fill(token, string, (int) -1, TLEFTPAR);
    else if (c == ')')
        token_fill(token, string, (int) -1, TRIGHTPAR);
    else if (c == ':')
        token_fill(token, string, (int) -1, TCOLON);
    else if (c == ',')
        token_fill(token, string, (int) -1, TCOMMA);
    else if (c == EOF)  // If EOF do nothing, do-while will end
        return;
    else    // Undifiend char
    {
        error_print("Undefined char on stdin", row, character_position);
        token_fill(token, string, (int) -1, TLEXERR);
        string = ptr_string_new();
    }
}

tToken get_token()
{
    tToken token;
    int c; // New char on input
    tState state = sStart;

    token = indent_counter();
    last_token_type = token.type;
    token.value = NULL;
    if(token.type != TNOTHING) //if indent wasn't the same as before
    {
        return token;
    }

    ptr_string_t string = ptr_string_new(); // Space for something readed from input

    do
    {
        c=getchar();
        ptr_string_append(string, c);

        character_position++;   // We've moved by one character

        switch(state)
        {
            /*************************** Start ********************************/   
            case sStart:
                start_state((char) c, &state, string, &token);
                if (token.value != NULL)
                    return token;
                break;

            case sNewLine:
                row++;  // We have moved one row
                return token_fill(&token, string, c, TNEWLINE);
                break;

            /*************************** Numbers ********************************/
            case sInteger0:
                if (c == '.')
                    state = sIntWithDot;
                else if (c == 'e' || c == 'E')
                    state = sExponent;
                else if (c >= '0' && c <= '9')  // Oct number is not valid
                {
                    error_print("Digit '0' must be followed by '.' or left alone", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                else    // Just zero
                    return token_fill(&token, string, c, TINT);
                break;
            case sInteger:
                if (c >= '0' && c <= '9')
                    state = sInteger;
                else if (c == '.')
                    state = sIntWithDot;
                else if (c == 'e' || c == 'E')
                    state = sExponent;
                else  // End of the int number
                     return token_fill(&token, string, c, TINT);
                break;
            case sIntWithDot:
                if (c >= '0' && c <= '9')
                    state = sFloat;
                else
                {
                    error_print("Number with dot (.) must be followed by number", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                break;
            case sFloat:
                if (c >= '0' && c <= '9')
                    state = sFloat;
                else if (c == 'e' || c == 'E')
                    state = sExponent;
                else   // End of the float number
                    return token_fill(&token, string, c, TFLOAT);
                break;
            case sExponent:
                if (c >= '0' && c <= '9')
                    state = sExponentNumber;
                else if (c == '-' || c == '+')
                    state = sExponentOperator;
                else    // Error (not ending state)
                {
                    error_print("Char 'E' as exponent must be followed by '- or +' or a number", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                break;
            case sExponentNumber:
                if (c >= '0' && c <= '9')
                    state = sExponentNumber;
                else
                {
                    char * string_number = ptr_string_c_string(string);
                    double number = atof(string_number);
                    free(string_number);
                    char buffer[500];
                    int check = sprintf(buffer, "%lf", number);
                    if (check < 0)  // Sprintf failed
                    {
                        error_print("Internal error", row, character_position);
                        return token_fill(&token, string, c, TERR);
                    }
                    if (strcmp(buffer, "inf") == 0)
                    {
                        error_print("Internal error", row, character_position);
                        return token_fill(&token, string, c, TERR);
                    }
                    string = ptr_string(buffer);
                    return token_fill(&token, string, c, TFLOAT);
                }
            case sExponentOperator:
                if (c >= '0' && c <= '9')
                    state = sExponentNumber;
                else
                {
                    error_print("Exponent must be followed by number", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                break;

            /*************************** Comments ********************************/
            case sLineComment:
                if (c == '\n')  // End of comment
                {
                    state = sNewLine;
                    ptr_string_delete(string);
                    string = ptr_string_new();
                }
                else
                    state = sLineComment;
                break;
            case sBlockCommentStart1:
                if (c == '"')   // It means now you have second '""'
                {
                    ptr_string_delete_last(string); // Delete second " from string
                    state = sBlockCommentStart2;
                }
                else
                {
                    error_print("Block comment is not valid", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                break;
            case sBlockCommentStart2:
                if (c == '"')   // It means now you have third '"""' and the comment has started
                {
                    ptr_string_delete_last(string); // Delete last " from string
                    state = sBlockComment;
                }
                else
                {
                    error_print("Block comment is not valid", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
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
                else if (c == EOF)     // If EOF, it's syntax error (Ending of the comment is missing)
                {
                    error_print("Block comment is not valid", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                else    // Everything inside the block comment
                    state = sBlockComment;
                break;
            case sBlockCommentEnd1:
                if (c == '\n')  // We have moved by one line
                {
                    row++;
                    character_position = 0; // New line so possition is 0
                }
                if (c == '"')   // It means now you have second '""'
                    state = sBlockCommentEnd2;
                else if (c == EOF)     // If EOF, it's lex error (Ending of the comment is missing)
                {
                    error_print("Block comment is not valid", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                else
                    state = sBlockComment;
                break;
            case sBlockCommentEnd2:
                if (c == '\n')  // We have moved by one line
                {
                    row++;
                    character_position = 0; // New line so possition is 0
                }
                if (c == '"')   // It means now you have third '"""' and the comment has ended
                {
                    for (short i = 0; i < 3; i++)   // Delete last three "
                    {
                        ptr_string_delete_last(string);
                    }
                    return token_fill(&token, string, (int) -1, TBLOCKCOMMENTORLITERAL);
                }   
                else if (c == EOF)     // If EOF, it's syntax error (Ending of the comment is missing)
                {
                    error_print("Block comment is not valid", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                else
                    state = sBlockComment;
                break;    
            
            /************************* Identificator/Keyword *****************************/
            case sIdentificatorOrKeyWord:
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
                    state = sIdentificatorOrKeyWord;
                else
                {
                    ptr_string_delete_last(string);
                    for (unsigned short i = 0; i < KEYWORDS_COUNT; i++) // Check for keyword
                    {
                        if (ptr_string_c_equals(string, keywords[i]))
                        {
                            return token_fill(&token, string, c, TKEYWORD);
                        }
                    }   // Not a keyword
                    return token_fill(&token, string, c, TIDENTIFICATOR);
                }
                break;
            
            /************************* Operators *****************************/
            // Add, Sub and Mul is already done in sStart state
            case sDivOrFloorDiv:
                if (c == '/')   // Floor division
                    return token_fill(&token, string, (int) -1, TFLOORDIV);
                else    // Regular division
                    return token_fill(&token, string, c, TDIV);
                break;
            case sAssignOrEqual:
                if (c == '=')   // Equal
                    return token_fill(&token, string, (int) -1, TEQ);
                else    // Assign
                    return token_fill(&token, string, c , TASSIGN);
                break;
            case sLtOrLte:
                if (c == '=')   // Less than or Equal
                    return token_fill(&token, string, (int) -1, TLTE);
                else    // Less than
                    return token_fill(&token, string, c , TLT);
                break;
            case sGtOrGte:
                if (c == '=')   // Greater than or Equal
                    return token_fill(&token, string, (int) -1, TGTE);
                else    // Greater than
                    return token_fill(&token, string, c , TGT);
                break;
            case sExclMark:
                if (c == '=')   // Not equal
                    return token_fill(&token, string, (int) -1, TNE);
                else    // Error
                {
                    error_print("Exclamation mark should be followed by equal sign", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                break;

            /************************* String *****************************/
            case sString:
                if (c > 31 && c != '\\' && c != '\'')
                    state = sString;
                else if (c == '\\')
                    state = sStringEscape;
                else if (c == 39) // End of the string
                    return token_fill(&token, string, (int) -1, TSTRING);   // -1 because we don't want to unget that char (')
                else
                {
                    error_print("The string literal is not complete", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                break;
            case sStringEscape:
                if (c == 'x')  // Hexa number
                    state = sStringEscapeNumber;
                else    // Not valid, nothing happens, also valid counts here
                    state = sString;
                break;
            case sStringEscapeNumber:
                if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
                    state = sStringEscapeNumber1;
                else
                {
                    error_print("Hexadecimal number in string must have two valid digits/symbols", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                break;
            case sStringEscapeNumber1:
                if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
                    // End of the hex number
                    state = sString;
                else
                {
                    error_print("Hexadecimal number in string must have two digits/symbols", row, character_position);
                    return token_fill(&token, string, c, TLEXERR);
                }
                break;
        }
    } while (c != EOF);    // Until whole input is readed
    stack_destroy(stack);
    ptr_string_delete(string);

    token.value = NULL;
    last_token_type = token.type = TENDOFFILE;
    return(token);
}