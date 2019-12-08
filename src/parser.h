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

parser_result_t parse();

#endif
