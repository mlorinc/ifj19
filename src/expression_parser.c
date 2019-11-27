
#include "expression_parser.h"
#include "stack.h"

#define table_size (7)

typedef enum precedent_element
{
    P1, // <, <=, >, >=, ==, !=
    P2, // +, -
    P3, // *, /, //
    Plp, // (
    Prp, // )
    Po, // constant, variable
    Pend // end of expression
}*precedent_e;

char precedent_table[table_size][table_size] =
{
//    P1  P2  P3 Plp Prp  Po Pend
    {'>','<','<','<','>','<','>'},  //P1 TODO problem with operation
    {'>','>','<','<','>','<','>'},  //P2
    {'>','>','>','<','>','<','>'},  //P3
    {'<','<','<','<','=','<',' '},  //Plp
    {'>','>','>',' ','>',' ','>'},  //Prp
    {'>','>','>',' ','>',' ','>'},  //Po
    {'<','<','<','<',' ','<',' '}   //Pend
};

int token_to_precedent_e(tToken_type token)
{
    switch(token)
    {
        case TINT:
        case TFLOAT:
        case TIDENTIFICATOR:
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

parser_result_t parse_expression(parser_t parser)
{
    stack_t stack = stack_init();
    tToken token = get_token();
    char operation;
    int precedent_elem = token_to_precedent_e(token.type);
    int* end_of_stack = malloc(sizeof(int));
    *end_of_stack = Pend;
    stack_push(stack, end_of_stack);
    int* top_of_stack = NULL;

    while(precedent_elem != Pend)
    {
        top_of_stack = stack_top(stack);
        operation = precedent_table[*top_of_stack][precedent_elem];

        switch(operation)
        {
            case ' ':
                queue_destroy();
                stack_destroy(stack);
                return NULL;
                break;
            case '=':
                stack_push(stack, token);
                break;
            case '<':
                //push < and token
                break;
            case '>':
                //pop while <
                break;
        }


        token = get_token();
        precedent_elem = token_to_precedent_e(token.type);
    }

    stack_destroy(stack);
}