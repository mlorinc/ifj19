#ifndef __H_PARSER
#define __H_PARSER

#include "scaner.h"
#include <string.h>

struct AST
{
    struct AST *left;
    struct AST *right;
    tToken token;
};

typedef struct AST *AST_T;

typedef struct parser
{
    tToken token;
    tToken previousToken;
} * parser_t;

AST_T node_init_empty();

AST_T node_init(tToken token);

parser_t parser_init();

void parser_destroy(parser_t parser);

void parser_next(parser_t parser);

bool accept(parser_t parser, tToken_type token_type);

AST_T factor(parser_t parser);

AST_T term(parser_t parser);

AST_T expression(parser_t parser);

AST_T parse();

#endif
