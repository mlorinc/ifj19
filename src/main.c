#include "parser.h"
#include "parser_result.h"
#include "parser_ast.h"
#include "semantic_analyser.h"
#include "generate.h"
#include <stdio.h>

int main(void)
{
    printf(".IFJcode19\n");
    printf("DEFVAR GF@TypeCheck1\n");
    printf("DEFVAR GF@TypeCheck2\n"); //Variables for typecheck in Functions
    printf("DEFVAR GF@VarForGTE\n");
    printf("DEFVAR GF@VarForLTE\n\n"); //Variables for GTE or LTE comparison
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

