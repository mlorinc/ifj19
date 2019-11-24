#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include "parser.h"
#include "queue.h"
#include "ptr_string.h"

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

typedef ast_t (*universal_term_function)(parser_t parser);

ast_t node_init_empty()
{
    ast_t ast = malloc(sizeof(struct ast));
    if (ast != NULL)
    {
        ast->nodes = queue_init();
    }
    return ast;
}

ast_t node_init(AST_node_type_t type)
{
    ast_t tree = node_init_empty();
    if (tree != NULL)
    {
        tree->node_type = type;
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
    do
    {
        if (parser->token.type == token_type)
        {
            parser_next(parser);
            return true;
        }
    } while (parser->token.type != TNEWLINE);
    return false;
}

ast_t factor(parser_t parser)
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

ast_t universal_term(parser_t parser, universal_term_function fn, const tToken_type operators[], size_t operators_length)
{
    ast_t f1 = fn(parser);
    if (f1 != NULL)
    {
        ast_t last_ast = node_init_empty();
        ast_t current_ast = node_init_empty();
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

                    ast_t f2 = fn(parser);
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

ast_t term(parser_t parser)
{
    tToken_type operators[] = {TMUL, TDIV, TFLOORDIV};
    return universal_term(parser, factor, operators, 3);
}

ast_t arithmetic_expression(parser_t parser)
{
    tToken_type operatos[] = {TADD, TSUB};
    return universal_term(parser, term, operatos, 2);
}

ast_t comparision(parser_t parser)
{
    tToken_type operators[] = {TLT, TGT, TEQ, TGTE, TLTE, TNE};
    return universal_term(parser, arithmetic_expression, operators, 6);
}

ast_t not_test(parser_t parser)
{
    if (accept(parser, TKEYWORD))
    {
        if (ptr_string_c_equals(parser->previousToken.value, "not"))
        {
            fprintf(stderr, "Line %ld: %s %s\n", line,
                    "Expecting not keyword, got", parser->previousToken.value);
            return NULL;
        }
        ast_t ast;
        tToken not_token = parser->previousToken;

        ast = not_test(parser);

        if (ast == NULL)
        {
            ast = comparision(parser);
            if (ast != NULL)
            {
                ast_t parent = node_init(not_token);
                parent->left = ast;
                return parent;
            }
            else
            {
                fprintf(stderr, "Line %ld: %s\n", line, "Expecting comparision");
                return NULL;
            }
        }
        else
        {
            return node_init(parser->previousToken);
        }
    }
    else
    {
        return NULL;
    }
}

ast_t and_test(parser_t parser)
{
    return universal_term(parser, not_test, TA);
}

ast_t flow_statement(parser_t parser)
{
    if (accept(parser, TKEYWORD))
    {
        if (ptr_string_c_equals(parser->previousToken.value, "break"))
        {
            return node_init(BREAK);
        }
    }
    if (accept(parser, TKEYWORD))
    {
        if (ptr_string_c_equals(parser->previousToken.value, "continue"))
        {
            return node_init(CONTINUE);
        }
    }
    if (accept(parser, TKEYWORD))
    {
        if (ptr_string_c_equals(parser->previousToken.value, "return"))
        {
            ast_t ret = node_init(RETURN);
            ast_t expr = expression(parser);
            if (expr != NULL)
            {
                queue_push(ret->node_type, expr);
                return ret;
            }
        }
    }
}

ast_t small_statement(parser_t parser)
{
    if (accept(parser, TKEYWORD))
    {
        if (ptr_string_c_equals(parser->previousToken.value, "pass"))
        {
            return node_init(PASS);
        }
    }

    ast_t flow_stmt = flow_statement(parser);

    if (flow_stmt != NULL)
    {
        return flow_stmt;
    }
}

ast_t simple_statement(parser_t parser)
{
    ast_t small_stmt = small_statement(parser);

    if (small_stmt != NULL)
    {
        if (accept(parser, TNEWLINE))
        {
            return small_stmt;
        }
    }
}

ast_t suite(parser_t parser)
{
    ast_t simple_stmt = simple_statement(parser);
    if (simple_stmt != NULL)
    {
        return simple_stmt;
    }
    else
    {
        if (accept(parser, TNEWLINE) && accept(parser, TINDENT))
        {
            ast_t consequent = node_init(CONSEQUENT);
            queue_t statements = queue_init();
            for (ast_t stmt = statement(parser); stmt != NULL; stmt = statement(parser))
            {
                queue_push(consequent->nodes, stmt);
            }

            // empty suite, we need at least 1
            if (queue_size(consequent->nodes) == 0)
            {
                // todo: error
            }

            if (accept(parser, TDEDENT))
            {
                return consequent;
            }
            else
            {
                // todo: error
            }
        }
        else
        {
            // todo: error
        }
    }
}

ast_t if_statement(parser_t parser, AST_node_type_t if_type)
{
    if (accept(parser, TKEYWORD))
    {
        char *tokenLiteral;

        switch (if_type)
        {
        case IF:
            tokenLiteral = "if";
            break;
        case ELIF:
            tokenLiteral = "elif";
            break;
        case ELSE:
            tokenLiteral = "else";
            break;
        default:
            // todo: error
            break;
        }

        ast_t if_stmt = node_init(if_type);

        if (ptr_string_c_equals(parser->previousToken.value, "else"))
        {
            if (accept(parser, TCOLON))
            {
                ast_t consequent = suite(parser);
                if (consequent != NULL)
                {
                    queue_push(if_stmt->nodes, consequent);
                    return if_stmt;
                }
                else
                {
                    // todo: error
                }
            }
            else
            {
                // todo: error
            }
        }
        if (ptr_string_c_equals(parser->previousToken.value, tokenLiteral))
        {
            ast_t comp = comparision(parser);

            if (comp != NULL)
            {
                queue_push(if_stmt->nodes, comp);
            }
            else
            {
                // todo: error
            }

            if (accept(parser, TCOLON))
            {
                ast_t consequent = suite(parser);
                if (consequent != NULL)
                {
                    queue_push(if_stmt->nodes, consequent);
                    ast_t elif_stmt = if_statement(parser, ELIF);
                    if (elif_stmt != NULL)
                    {
                        queue_push(if_stmt->nodes, elif_stmt);
                    }
                    return if_stmt;
                }
                else
                {
                    // todo: error
                }
            }
            else
            {
                // todo: error
            }
        }
    }
    else if (if_type != ELIF)
    {
        // todo: error
    }
    else
    {
        return NULL;
    }
}

ast_t while_statement(parser_t parser)
{
    if (accept(parser, TKEYWORD))
    {
        if (ptr_string_c_equals(parser->previousToken.value, "while"))
        {
            ast_t while_stmt = node_init(WHILE);
            ast_t comp = comparision(parser);

            if (comp != NULL)
            {
                queue_push(while_stmt->nodes, comp);

                ast_t suites = suite(parser);

                if (suites != NULL)
                {
                    queue_push(while_stmt->nodes, suites);

                    if (accept(parser, TKEYWORD))
                    {
                        if (ptr_string_c_equals(parser->previousToken.value, "else"))
                        {
                            if (accept(parser, TCOLON))
                            {
                                suites = suite(parser);
                                if (suites != NULL)
                                {
                                    queue_push(while_stmt->nodes, suites);
                                    return while_stmt;
                                }
                                else
                                {
                                    // todo: error
                                }
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                            // todo: set current token to previous
                            return while_stmt;
                        }
                    }
                    else
                    {
                        return while_stmt;
                    }
                }
                else
                {
                    // todo: error
                }
            }
            else
            {
                // todo: error
            }
        }
        else
        {
            // todo: error
            return NULL;
        }
    }
    return NULL;
}

ast_t compound_statement(parser_t parser)
{
    ast_t stmt;
    stmt = if_statement(parser, IF);
    if (stmt == NULL)
    {
        stmt = while_statement(parser);
    }
    else if (stmt == NULL)
    {
        stmt = functionDef(parser);
    }
    return stmt;
}

ast_t statement(parser_t parser)
{
    ast_t stmt;
    stmt = simple_statement(parser);
    if (stmt == NULL)
    {
        stmt = compound_statement(parser);
    }
    return stmt;
}

ast_t parse()
{
    parser_t parser = parser_init();
    parser_next(parser);

    ast_t ast = statement(parser);

    parser_destroy(parser);
    return ast;
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
ast_t functionDef(parser_t parser, queue_t queue)
{
    ast_t ast = node_init_empty();
    if (accept(parser, TKEYWORD))
    {
        if (ptr_string_c_equals(parser->previousToken.value, "def"))
        {
            stderr_print(line, TKEYWORD, parser);
            return NULL;
        }

        if (accept(parser, TIDENTIFICATOR))
        {
            ast_t parent = node_init(parser->previousToken);
            if (accept(parser, TLEFTPAR))
            {
                if (parser->token.type == TIDENTIFICATOR)
                {
                    functionParams(parser, ast);
                }
                else if (accept(parser, TRIGHTPAR))
                {
                    if (accept(parser, TCOLON))
                    {
                        if (parser->token.type == TENDOFLINE)
                        {
                            return ast;
                        }
                        else
                        {
                            stderr_print(line, TNEWLINE, parser);
                            return NULL;
                        }
                    }
                    else
                    {
                        stderr_print(line, TKEYWORD, parser);
                        return NULL;
                    }
                }
                else
                {
                    stderr_print(line, TRIGHTPAR, parser);
                    return NULL;
                }
            }
            else
            {
                stderr_print(line, TLEFTPAR, parser);
                return NULL;
            }
        }
        else
        {
            stderr_print(line, TIDENTIFICATOR, parser);
            return NULL;
            //lubim vinciho je to moje bubu
        }
    }

    return NULL;
}

ast_t functionParams(parser_t parser, ast_t ast)
{
}

void stderr_print(long int line, tToken type, parser_t parser)
{
    fprintf(stderr, "Line %ld: %s %u %s %s\n", line,
            "Expecting ", type.type, " got", (char *)parser->previousToken.value);
    parser_destroy(parser);
    return NULL;
}
