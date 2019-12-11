#include "parser.h"
#include "parser_result.h"
#include "parser_ast.h"
#include "semantic_analyser.h"
#include "generate.h"
#include <stdio.h>

int main(void)
{
    parser_result_t result = parse();

    if (result.error_code != ERROR_OK) {
        if (result.ast != NULL) {
            ast_delete(result.ast);
        }
        return result.error_code;
    }

    semantic_result_t semantic = semantic_analysis(result.ast);
    if (semantic.status != ERROR_OK) {
        if (semantic.ast != NULL) {
            ast_delete(result.ast);
        }
        return semantic.status;
    }

    generate(result.ast);

    return ERROR_OK;
}

