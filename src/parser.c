#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "parser.h"
#include "scaner.h"

AST_T node_init(tToken token);

typedef struct parser
{
    tToken token;
    tToken previousToken;
} * parser_t;


AST_T node_init_empty() {
    return malloc(sizeof(struct AST));
}

AST_T node_init(tToken token)
{
    AST_T tree = node_init_empty();
    if (tree != NULL)
    {
        tree->token = token;
    }
    return tree;
}

parser_t parser_init()
{
    return malloc(sizeof(struct parser));
}

void parser_destroy(parser_t parser)
{
    assert(parser != NULL);
    free(parser);
}

void parser_next(parser_t parser)
{
    assert(parser != NULL);
    parser->previousToken = parser->token;
    parser->token = getToken();
}

bool accept(parser_t parser, tToken_type token_type)
{
    if (parser->token.type == token_type)
    {
        parser_next(parser);
        return true;
    }
    return false;
}

AST_T factor(parser_t parser)
{
    if (accept(parser, TADD) || accept(parser, TSUB))
    {
        tToken operator= parser->previousToken;
        if (accept(parser, TFLOAT) || accept(parser, TINT))
        {
            tToken number = parser->previousToken;
            if (operator.type == TSUB)
            {
                DEREFENCE_AS(number.value, int) = -DEREFENCE_AS(number.value, int);
            }
            return node_init(number);
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

AST_T universal_term(parser_t parser, )

AST_T term(parser_t parser) {
    AST_T f1 = factor(parser);
    if (f1 != NULL) {
        AST_T last_ast = node_init_empty();
        AST_T current_ast = node_init_empty();
        current_ast->left = f1;
        for (;;)
        {
            if (accept(parser, TMUL) || accept(parser, TDIV) || accept(parser, TMOD))
            {
                current_ast->token = parser->previousToken;

                AST_T f2 = factor(parser);
                if (f2 != NULL) {
                    current_ast->right = f2;
                }
                else {
                    return NULL;
                }
                
                last_ast = current_ast;
                current_ast = node_init_empty();

                current_ast->left = last_ast;
            }
            else {
                return current_ast;
            }
        }
        
    }
    else {
        return NULL;
    }
}

AST_T expression(parser_t parser)
{
    if (accept(parser, TFLOAT) || accept(parser, TINT))
    {
        return node_init(parser->previousToken);
    }
    if (accept(parser, TLEFTPAR))
    {
        AST_T expr = expression(parser);
        if (expr != NULL)
        {
            if (accept(parser, TRIGHTPAR))
            {
                return expr;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }
    return NULL;
}

AST_T parse()
{
    parser_t parser = parser_init();
    parser_next(parser);

    parser_destroy(parser);
}
