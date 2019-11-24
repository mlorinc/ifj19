#ifndef __H_PARSER
#define __H_PARSER

#include "scaner.h"
#include "queue.h"
#include "ptr_string.h"
#include "parser_ast.h"

typedef struct parser
{
    tToken token;
    tToken previousToken;
    queue_t returned_tokens;
} * parser_t;

parser_t parser_init();

void parser_destroy(parser_t parser);

void parser_next(parser_t parser);

bool accept(parser_t parser, tToken_type token_type);

void parser_return_back(parser_t parser, tToken token);

ast_t factor(parser_t parser);

ast_t term(parser_t parser);

ast_t expression(parser_t parser);

ast_t parse();

ast_t functionDef(parser_t parser, queue_t queue);

ast_t functionParams(parser_t parser, ast_t ast);

void stderr_print(long int line, tToken type, parser_t parser);

#endif
