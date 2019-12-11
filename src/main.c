#include "parser.h"
#include "parser_result.h"
#include "parser_ast.h"
#include "semantic_analyser.h"
#include "generate.h"
#include "generator_functions.h"
#include <stdio.h>

int main(void)
{
    printf(".IFJcode19\n");
    printf("DEFVAR GF@TypeCheck1\n");
    printf("DEFVAR GF@TypeCheck2\n"); //Variables for typecheck in Functions
    printf("DEFVAR GF@RecastVar1\n");
    printf("DEFVAR GF@RecastVar2\n");
    printf("DEFVAR GF@RecastVar3\n");
    printf("DEFVAR GF@WhatType1\n");
    printf("DEFVAR GF@WhatType2\n\n");

    printf("JUMP $$main\n\n");

    generate_semantic_check_add();
    generate_semantic_check_sub();
    generate_semantic_check_mul();
    generate_semantic_check_div();
    generate_semantic_check_idiv();

    printf("LABEL $$main\n"
           "DEFVAR GF@a\n"
           "CREATEFRAME\n");
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

    printf("WRITE GF@a\n");

    return ERROR_OK;
}

