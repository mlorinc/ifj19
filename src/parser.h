#ifndef __H_PARSER
#define __H_PARSER

#include "scaner.h"
#include "queue.h"

typedef enum {
    PASS,
    RETURN,
    BREAK,
    CONTINUE,
    FUNCTION,
    FUNCTION_DEFINITION,
    ID,
    CONSTANT,
    PLUS_OP,
    MINUS_OP,
    MULTIPLY_OP,
    DIVIDE_OP,
    FLOOR_DIVIDE_OP,
    EQUALS,
    ASSIGN,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_EQUAL,
    GREATER_THAN_EQUAL,
    NOT_EQUAL,
    WHILE,
    IF,
    ELIF,
    ELSE,
    NONE
} AST_node_type_t;

struct AST
{
    queue_t nodes;
    AST_node_type_t node_type;
};

typedef struct AST *AST_T;

typedef struct parser
{
    tToken token;
    tToken previousToken;
} * parser_t;

AST_T node_init_empty();

AST_T node_init(AST_node_type_t type);

parser_t parser_init();

void parser_destroy(parser_t parser);

void parser_next(parser_t parser);

bool accept(parser_t parser, tToken_type token_type);

AST_T factor(parser_t parser);

AST_T term(parser_t parser);

AST_T expression(parser_t parser);

AST_T parse();

AST_T functionDef(parser_t parser, queue_t queue);

AST_T functionParams(parser_t parser, AST_T ast);

void stderr_print(long int line, tToken type, parser_t parser);

#endif
