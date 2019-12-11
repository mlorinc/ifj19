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
    printf("DEFVAR GF@RecastVar1\n");
    printf("DEFVAR GF@RecastVar2\n");
    printf("DEFVAR GF@RecastVar3\n");
    printf("DEFVAR GF@WhatType1\n\n");
    printf("DEFVAR GF@WhatType2\n\n");

    printf("JUMP $$main\n\n");

    void generate_semantic_check_add();
    void generate_semantic_check_sub();
    void generate_semantic_check_mul();
    void generate_semantic_check_div();
    void generate_semantic_check_idiv();

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

