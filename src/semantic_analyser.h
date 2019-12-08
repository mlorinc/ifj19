#ifndef __H_SEMANTIC_ANALYSER
#define __H_SEMANTIC_ANALYSER

#include "parser_ast.h"
#include "parser_result.h"
#include "scope.h"
#include "error.h"

typedef struct semantic_result
{
    ast_t ast;
    scope_t scope;
    enum error_codes status;
}
semantic_result_t;

semantic_result_t semantic_analysis(ast_t root);

#endif
