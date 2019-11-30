/**
 * IFJ2019 @ VUT FIT Brno
 * 30.11.2019
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
 * File: expression_parser.c
 *
 */

#include "expression_parser.h"
#include "stack.h"
#include "parser_ast.h"
#include "queue.h"
#include "ptr_string.h"
#include <stdbool.h>

#define table_size (7)

enum precedent_element
{
    P1, // <, <=, >, >=, ==, !=
    P2, // +, -
    P3, // *, /, //
    Plp, // (
    Prp, // )
    Po, // constant, variable
    Pend // end of expression
};

char precedent_table[table_size][table_size] =
{
//    P1  P2  P3 Plp Prp  Po Pend
    {'>','<','<','<','>','<','>'},  //P1 TODO problem with operation
    {'>','>','<','<','>','<','>'},  //P2
    {'>','>','>','<','>','<','>'},  //P3
    {'<','<','<','<','=','<',' '},  //Plp
    {'>','>','>',' ','>',' ','>'},  //Prp
    {'>','>','>',' ','>',' ','>'},  //Po
    {'<','<','<','<',' ','<','#'}   //Pend
};

typedef struct table_value
{  
    enum precedent_element element;
    tToken* token;
}* p_table_value;

/**
 * Convert token to precedent element.
 * @param token converting token
 * @return element type
 */
int token_to_precedent_e(tToken_type token)
{
    switch(token)
    {
        case TINT:
        case TFLOAT:
        case TIDENTIFICATOR:
        case TSTRING:
            return Po;
        case TLT:
        case TGT:
        case TLTE:
        case TGTE:
        case TEQ:
        case TNE:
            return P1;
        case TADD:
        case TSUB:
            return P2;
        case TMUL:
        case TDIV:
        case TFLOORDIV:
            return P3;
        case TLEFTPAR:
            return Plp;
        case TRIGHTPAR:
            return Prp;
        default:
            return Pend;
    }
}

/**
 * Create table value which will be store on stack
 * @param token value for table_value
 * @param element value for table_value
 * @return pointer on table_value
 */
p_table_value create_table_value(tToken token, enum precedent_element element)
{
    p_table_value new = malloc(sizeof(struct table_value));
    if(new != NULL)
    {
        new->element = element;
        tToken* ptoken = malloc(sizeof(tToken)); 
        if(ptoken != NULL)
        {
            ptoken->type = token.type;
            ptoken->value = token.value;
        }
        new->token = ptoken;
    }
    return new;
}

/**
 * Do precedent analyze
 * @param precedent_stack stack for precedent analyze
 * @param postfix output queue for tokens
 * @param new_value table_value on input
 * @return if syntactic error return false, else true
 */
bool read_table(stack_t precedent_stack, queue_t postfix, p_table_value new_value)
{
    p_table_value top_on_stack = stack_top(precedent_stack);
    char operation = precedent_table[top_on_stack->element][new_value->element];
    switch(operation)
    {
        case '<':
            stack_push(precedent_stack, new_value);
            return true;
        case '>':
            if(top_on_stack->element != Prp && top_on_stack->element != Plp)
            {
                queue_push(postfix, top_on_stack->token);
            }
            stack_pop(precedent_stack);
            if(top_on_stack->element == Prp)
            {
                if(((p_table_value) stack_top(precedent_stack))->element == Plp)
                {
                    free(top_on_stack);
                    top_on_stack = stack_pop(precedent_stack);
                }
            }
            free(top_on_stack);
            return read_table(precedent_stack, postfix, new_value);
        case '=':
            stack_push(precedent_stack, new_value);
            return true;
        case ' ':
            while (!stack_empty(precedent_stack))
            {
                stack_pop(precedent_stack);
                free(top_on_stack->token);
                free(top_on_stack);
                top_on_stack = stack_pop(precedent_stack);
            }
            deque_destroy(postfix);
            stack_destroy(precedent_stack);
            return false;
        default:
            return true;
    }
}

parser_result_t parse_expression(parser_t parser)
{
    queue_t postfix = queue_init();
    stack_t precedent_stack = stack_init();

    ast_t ast = malloc(sizeof(struct ast));
    ast->nodes = NULL;
    ast->node_type = EXPRESSION;
    ast->data = postfix;

    parser_result_t result;
    result.ast = ast;
    result.error = NULL;

    tToken token = get_token();
    p_table_value table_value = create_table_value(token, token_to_precedent_e(token.type));

    parser->previousToken = parser->token;
    parser->token = token;
    queue_push(parser->returned_tokens, table_value->token);

    tToken end_of_stack;
    end_of_stack.type = TNOTHING;
    end_of_stack.value = NULL;
    stack_push(precedent_stack, create_table_value(end_of_stack, Pend));

    p_table_value top_of_stack;

    do
    {
        if(!read_table(precedent_stack, postfix, table_value))
        {
            result.ast->data = NULL;
            result.error = ptr_string("Expression error.");
            return result;
        }

        if(table_value->element != Pend)    //  If token on input is still expression token.
        {
            token = get_token();
            table_value = create_table_value(token, token_to_precedent_e(token.type));

            parser->previousToken = parser->token;
            parser->token = token;
            queue_push(parser->returned_tokens, table_value->token);
        }
        top_of_stack = stack_top(precedent_stack);
    }while(top_of_stack->element != Pend || table_value->element != Pend);

    int last_expresion_element = token_to_precedent_e(parser->previousToken.type);

    if(queue_empty(postfix))    //  Expression wasn't found.
    {
        queue_destroy(postfix);
        result.ast->data = NULL;
        result.error = ptr_string("Not an expression.");
    }
    else if(last_expresion_element != Prp && last_expresion_element != Po)
    {
        queue_destroy(postfix);
        result.ast->data = NULL;
        result.error = ptr_string("Expression error.");
    }

    stack_destroy(precedent_stack);
    return result;
}
