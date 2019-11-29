#ifndef __H_PARSER_RESULT
#define __H_PARSER_RESULT

#include "error.h"
#include "parser_ast.h"
#include "ptr_string.h"

struct parser_result {
    ast_t ast;
    ptr_string_t error;
    enum error_codes error_code;
};

typedef struct parser_result parser_result_t;

parser_result_t parser_result(ast_t ast);
parser_result_t parser_error_ptr_string(ast_t ast, ptr_string_t error);
parser_result_t parser_error(ast_t ast, const char *format, ...);
bool parser_error_dispose(parser_result_t parser_error);

#endif
