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

typedef enum precedent_element
{
    P1,  // <, <=, >, >=, ==, !=
    P2,  // +, -
    P3,  // *, /, //
    Plp, // (
    Prp, // )
    Po,  // constant, variable
    Pend // end of expression
} precedent_element;

char precedent_table[table_size][table_size] =
    {
        //    P1  P2  P3 Plp Prp  Po Pend
        {'>', '<', '<', '<', '>', '<', '>'}, //P1
        {'>', '>', '<', '<', '>', '<', '>'}, //P2
        {'>', '>', '>', '<', '>', '<', '>'}, //P3
        {'<', '<', '<', '<', '=', '<', ' '}, //Plp
        {'>', '>', '>', ' ', '>', ' ', '>'}, //Prp
        {'>', '>', '>', ' ', '>', ' ', '>'}, //Po
        {'<', '<', '<', '<', ' ', '<', '#'}  //Pend
};

typedef struct table_value
{
    precedent_element element;
    tToken *token;
} * p_table_value;

/**
 * Convert token to precedent element.
 * @param token converting token
 * @return element type
 */
precedent_element token_to_precedent_elem(tToken_type token)
{
    switch (token)
    {
        case TINT:
        case TFLOAT:
        case TIDENTIFICATOR:
        case TSTRING:
        case TBLOCKCOMMENTORLITERAL:
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
    if (new != NULL)
    {
        new->element = element;
        tToken *ptoken = malloc(sizeof(tToken));
        if (ptoken != NULL)
        {
            ptoken->type = token.type;
            ptoken->value = token.value;
        }
        new->token = ptoken;
    }
    return new;
}

/**
 * Push token on stack with meaning of end of precedent stack end.
 * @return stack for precedent analyze
 */
stack_t expression_stack_init()
{
    stack_t stack = stack_init();
    tToken end_of_stack;
    end_of_stack.type = TNOTHING;
    end_of_stack.value = NULL;
    stack_push(stack, create_table_value(end_of_stack, Pend));
    return stack;
}

/**
 * Destroy expression stack.
 * @param stack to destroy
 */
void expression_stack_destroy(stack_t stack)
{
    p_table_value tmp;
    while (!stack_empty(stack))
    {
        tmp = stack_pop(stack);
        free(tmp->token);
        free(tmp);
    }
    stack_destroy(stack);
}

/**
 * Destroy value on top of stack. If on top is ')' and
 * the second value is '(', this will be also destoy.
 * @param stack to destroy value on top
 */
void expression_stack_top_destoy(stack_t stack)
{
    p_table_value top_of_stack = stack_pop(stack);
    if (top_of_stack->element == Prp)
    {
        free(top_of_stack);
        top_of_stack = stack_top(stack);
        if (top_of_stack->element == Plp)
        {
            stack_pop(stack);
            free(top_of_stack);
        }
    }
    else
    {
        free(top_of_stack);
    }
}

/**
 * Do precedent analyze.
 * @param expression_stack stack for precedent analyze
 * @param postfix output queue for tokens
 * @param new_value table_value on input
 * @return if syntactic error return false, else true
 */
bool precedent_analyze(stack_t expression_stack, queue_t postfix, p_table_value new_value)
{
    p_table_value top_of_stack = stack_top(expression_stack);
    char operation = precedent_table[top_of_stack->element][new_value->element];
    switch (operation)
    {
    case '<':
    case '=':
        stack_push(expression_stack, new_value);
        return true;
    case '>':
        if (top_of_stack->element != Prp && top_of_stack->element != Plp) //If on top of stack isn't ')' or '('
        {
            queue_push(postfix, top_of_stack->token);
        }
        expression_stack_top_destoy(expression_stack);
        return precedent_analyze(expression_stack, postfix, new_value);
    case ' ':
        return false;
    default:
        return true;
    }
}

parser_result_t parse_expression(parser_t parser)
{
    stack_t expression_stack = expression_stack_init();
    queue_t postfix = queue_init();
    p_table_value new_table_value;

    do
    {
        new_table_value = create_table_value(parser->token, token_to_precedent_elem(parser->token.type));

        if ((parser->token.type == TRIGHTPAR && parser->previousToken.type == TLEFTPAR) ||
            !precedent_analyze(expression_stack, postfix, new_table_value)) // Bad expression.
        {
            expression_stack_destroy(expression_stack);
            queue_destroy(postfix);
            return parser_error(NULL, "Expression error on line %u.\n", parser->previousToken.line);
        }

        parser_next(parser);
    } while (new_table_value->element != Pend);

    parser_return_back(parser, parser->token);
    expression_stack_destroy(expression_stack);

    if (queue_empty(postfix)) //  Expression wasn't found.
    {
        queue_destroy(postfix);
        return parser_result(NULL);
    }

    precedent_element last_expresion_element = token_to_precedent_elem(parser->previousToken.type);

    if (last_expresion_element != Prp && last_expresion_element != Po) //  Bad end of expression.
    {
        queue_destroy(postfix);
        return parser_error(NULL, "Expression error on line %u.\n", parser->previousToken.line);
    }
    else
    {
        return parser_result(ast_node_init(EXPRESSION, parser->previousToken.line, parser->previousToken.pos, postfix));
    }
}
