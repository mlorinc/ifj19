#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "parser.h"
#include "queue.h"
#include "ptr_string.h"

typedef AST_T (*universal_term_function)(parser_t parser);

AST_T node_init_empty()
{
    AST_T ast = malloc(sizeof(struct AST));
    if (ast != NULL) {
        ast->nodes = queue_init();
    }
    return ast;
}

AST_T node_init(tToken token)
{
    AST_T tree = node_init_empty();
    if (tree != NULL)
    {
        tree->token = token;
    }
    return tree;
}

parser_t parser_init()
{
    return malloc(sizeof(struct parser));
}

void parser_destroy(parser_t parser)
{
    assert(parser != NULL);
    free(parser);
}

void parser_next(parser_t parser)
{
    assert(parser != NULL);
    parser->previousToken = parser->token;
    parser->token = get_token();
}

bool accept(parser_t parser, tToken_type token_type)
{
    if (parser->token.type == token_type)
    {
        parser_next(parser);
        return true;
    }
    return false;
}

bool expect(parser_t parser, tToken_type token_type)
{
    if (accept(parser, token_type))
    {
        return true;
    }
    fprintf(stderr, "%s\n", "unexpected token");
    return false;
}

AST_T factor(parser_t parser)
{
    if (accept(parser, TADD) || accept(parser, TSUB))
    {
        tToken operator= parser->previousToken;
        if (accept(parser, TFLOAT) || accept(parser, TINT))
        {
            tToken number = parser->previousToken;
            if (operator.type == TSUB)
            {
                DEREFENCE_AS(number.value, int) = -DEREFENCE_AS(number.value, int);
            }
            return node_init(number);
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

AST_T universal_term(parser_t parser, universal_term_function fn, const tToken_type operators[], size_t operators_length)
{
    AST_T f1 = fn(parser);
    if (f1 != NULL)
    {
        AST_T last_ast = node_init_empty();
        AST_T current_ast = node_init_empty();
        current_ast->left = f1;
        for (;;)
        {
            bool accepted = false;
            for (size_t i = 0; i < operators_length; i++)
            {
                tToken_type op = operators[i];
                if (accept(parser, op))
                {
                    current_ast->token = parser->previousToken;

                    AST_T f2 = fn(parser);
                    if (f2 != NULL)
                    {
                        current_ast->right = f2;
                    }
                    else
                    {
                        fprintf(stderr, "Line %ld: %s\n", line, "Missing second operand");
                        return NULL;
                    }

                    last_ast = current_ast;
                    current_ast = node_init_empty();

                    current_ast->left = last_ast;
                    accepted = true;
                    break;
                }
            }
            if (!accepted)
            {
                return current_ast;
            }
        }
    }
    else
    {
        fprintf(stderr, "Line %ld: %s\n", line, "Missing first operand");
        return NULL;
    }
}

AST_T term(parser_t parser)
{
    tToken_type operators[] = {TMUL, TDIV, TFLOORDIV};
    return universal_term(parser, factor, operators, 3);
}

AST_T arithmetic_expression(parser_t parser)
{
    tToken_type operatos[] = {TADD, TSUB};
    return universal_term(parser, term, operatos, 2);
}

AST_T comparision(parser_t parser)
{
    tToken_type operators[] = {TLT, TGT, TEQ, TGTE, TLTE, TNE};
    return universal_term(parser, arithmetic_expression, operators, 6);
}

AST_T not_test(parser_t parser) {
    if (accept(parser, TKEYWORD)) {
        if (ptr_string_c_equals(parser->previousToken.value, "not"))
        {
            fprintf(stderr, "Line %ld: %s %s\n", line,
                    "Expecting not keyword, got", parser->previousToken.value);
            return NULL;
        }
        AST_T ast;
        tToken not_token = parser->previousToken;

        ast = not_test(parser);

        if (ast == NULL) {
            ast = comparision(parser);
            if (ast != NULL) {
                AST_T parent = node_init(not_token);
                parent->left = ast;
                return parent;
            }
            else {
                fprintf(stderr, "Line %ld: %s\n", line, "Expecting comparision");
                return NULL;
            }
        }
        else {
            return node_init(parser->previousToken);
        }
    }
    else {
        return NULL;
    }
}

AST_T and_test(parser_t parser) {
    return universal_term(parser, not_test, TA);
}

AST_T parse()
{
    parser_t parser = parser_init();
    parser_next(parser);

    queue_t queue = queue_init();

    parser_destroy(parser);
    queue_destroy(queue);
    return NULL;
}

/**
 *                            FNC NAME
 *                              /  \
 *                             /   \
 *                            /    \
 *                        PARAMS   BODY
 *                          /        \
 *                         /         \
 *                        /          \
 *               list of params    body of function
 */
AST_T functionDef(parser_t parser, queue_t queue)
{
    AST_T ast = node_init_empty();
    if (accept(parser, TKEYWORD)) {
        if (ptr_string_c_equals(parser->previousToken.value, "def")) {
            stderr_print(line, TKEYWORD, parser);
            return NULL;
        }

        if (accept(parser, TIDENTIFICATOR)){
            AST_T parent = node_init(parser->previousToken);
            if (accept(parser, TLEFTPAR)){
                if (parser->token.type == TIDENTIFICATOR){
                    functionParams(parser, ast);
                }
                else if (accept(parser, TRIGHTPAR)){
                    if (accept(parser, TCOLON)){
                        if (parser->token.type == TENDOFLINE) {
                            return ast;
                        }
                        else{
                            stderr_print(line, TNEWLINE, parser);
                            return NULL;
                        }
                    }
                    else{
                        stderr_print(line, TKEYWORD, parser);
                        return NULL;
                    }
                }
                else {
                    stderr_print(line, TRIGHTPAR, parser);
                    return NULL;
                }
            }
            else{
                stderr_print(line, TLEFTPAR, parser);
                return NULL;
            }
        }
        else {
            stderr_print(line, TIDENTIFICATOR, parser);
            return NULL;
            //lubim vinciho je to moje bubu
        }
    }

    return NULL;
}

AST_T functionParams(parser_t parser, AST_T ast)
{


}

void stderr_print(long int line, tToken type, parser_t parser)
{
    fprintf(stderr, "Line %ld: %s %u %s %s\n", line,
            "Expecting ", type.type, " got", (char*)parser->previousToken.value);
}
