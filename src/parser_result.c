#include <stdarg.h>
#include <stdio.h>
#include "parser_result.h"

parser_result_t parser_result(ast_t ast) {
    parser_result_t result;
    result.ast = ast;
    result.error = NULL;
    return result;
}

parser_result_t parser_error_ptr_string(ast_t ast, ptr_string_t error) {
    parser_result_t result;
    result.ast = ast;
    result.error = error;
    return result;
}

parser_result_t parser_error(ast_t ast, const char *format, ...) {
    va_list list;
    va_start(list, format);
    char buffer[4096] = { 0 };
    // todo: possible buffer overflow vulnerability
    vsprintf(buffer, format, list);
    

    return parser_error_ptr_string(ast, ptr_string(buffer));
}

bool parser_error_dispose(parser_result_t parser_error) {
    if (parser_error.error != NULL) {
        ptr_string_delete(parser_error.error);
    }
    if (parser_error.ast != NULL) {
        ast_delete(parser_error.ast);
    }
    return true;
}
