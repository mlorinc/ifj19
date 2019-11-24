#include <stdarg.h>
#include "parser_result.h"

parser_result_t parser_result(ast_t ast) {
    parser_result_t result;
    result.ast = ast;
    result.error = NULL;
    return result;
}
parser_result_t parser_error(ast_t ast, const char *format, ...) {
    va_list list;
    va_start(list, format);
    char buffer[4096] = { 0 };
    // todo: possible buffer overflow vulnerability
    vsprintf(buffer, format, list);
    
    parser_result_t result;
    result.ast = ast;
    result.error = ptr_string(buffer);
    return result;
}

bool parser_error_dispose(parser_result_t parser_error) {
    if (parser_error.error != NULL) {
        return ptr_string_delete(parser_error.error);
    }
    return true;
}
