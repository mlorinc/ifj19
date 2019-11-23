#ifndef __H_PARSER
#define __H_PARSER

#include "scaner.h"
#include "queue.h"
#include "ptr_string.h"

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
    NONE,
    EXPRESSION,
    CONSEQUENT
} AST_node_type_t;

struct ast
{
    queue_t nodes;
    AST_node_type_t node_type;
};


typedef struct ast *ast_t;
typedef struct parser_result parser_result_t;

struct parser_result {
    ast_t ast;
    ptr_string_t error;
};

typedef struct parser
{
    tToken token;
    tToken previousToken;
} * parser_t;

ast_t node_init_empty();

ast_t node_init(AST_node_type_t type);

parser_t parser_init();

void parser_destroy(parser_t parser);

void parser_next(parser_t parser);

parser_result_t parser_result(ast_t ast);
parser_result_t parser_error(ast_t ast, const char *format, ...);
bool parser_error_dispose(parser_result_t parser_error);

bool accept(parser_t parser, tToken_type token_type);

ast_t factor(parser_t parser);

ast_t term(parser_t parser);

ast_t expression(parser_t parser);

ast_t parse();

ast_t functionDef(parser_t parser, queue_t queue);

ast_t functionParams(parser_t parser, ast_t ast);

void stderr_print(long int line, tToken type, parser_t parser);

#endif
