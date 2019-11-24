#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include "parser.h"
#include "queue.h"
#include "ptr_string.h"
#include "parser_result.h"
#include "expression_parser.h"

typedef parser_result_t (*parser_method_t)(parser_t parser);

parser_t parser_init()
{
    parser_t parser = malloc(sizeof(struct parser));

    if (parser == NULL)
    {
        return NULL;
    }

    parser->returned_tokens = queue_init();

    if (parser->returned_tokens == NULL)
    {
        free(parser);
        return NULL;
    }

    return parser;
}

void parser_destroy(parser_t parser)
{
    assert(parser != NULL);
    free(parser->returned_tokens);
    free(parser);
}

void parser_next(parser_t parser)
{
    assert(parser != NULL);
    parser->previousToken = parser->token;

    if (queue_empty(parser->returned_tokens))
    {
        parser->token = get_token();
    }
    else
    {
        tToken token = DEREFENCE_AS(queue_front(parser->returned_tokens), tToken);
        free(queue_pop(parser->returned_tokens));
        parser->token = token;
    }
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

bool accept_keyword(parser_t parser, const char *keyword)
{
    if (parser->token.type == TKEYWORD && ptr_string_c_equals(parser->previousToken.value, keyword))
    {
        parser_next(parser);
        return true;
    }
    return false;
}

void parser_return_back(parser_t parser, tToken token)
{
    tToken *t = malloc(sizeof(tToken));

    t->value = token.value;
    t->type = token.type;

    queue_push(parser->returned_tokens, t);
}

parser_result_t flow_statement(parser_t parser)
{
    if (accept_keyword(parser, "break"))
    {
        return parser_result(ast_node_init(BREAK, parser->previousToken.value));
    }
    else if (accept_keyword(parser, "continue"))
    {
        return parser_result(ast_node_init(CONTINUE, parser->previousToken.value));
    }
    else if (accept_keyword(parser, "return"))
    {
        // create return node
        ast_t ret = ast_node_init(RETURN, parser->previousToken.value);

        // try to get expression
        parser_result_t expr = parse_expression(parser);

        // check if error occurred
        if (expr.error)
        {
            // propagate error
            return expr;
        }

        if (expr.ast != NULL)
        {
            // found expression, add it to return node
            ast_add_node(ret, expr.ast);
        }
        else
        {
            // expresion not found, add None type
            ast_add_node(ret, ast_node_init(NONE, NULL));
        }

        return parser_result(ret);
    }
    else
    {
        // we did not find flow statement
        return parser_result(NULL);
    }
}

parser_result_t assign_statemnt(parser_t parser)
{
    if (accept(parser, TIDENTIFICATOR))
    {
        tToken id = parser->previousToken;
        if (accept(parser, TASSIGN))
        {
            // try to get expression
            parser_result_t expr = parse_expression(parser);

            // check if error occurred
            if (expr.error)
            {
                // something bad happened, we dont want to return back token,
                // we have to continue with parsing
                return expr;
            }

            // check if we found expression
            if (expr.ast == NULL)
            {
                // thats bad, we must return error
                return parser_error(NULL, "Invalid assignment, missing expression on right hand side\n");
            }
            else
            {
                // we are ok, lets create tree
                ast_t assign = ast_node_init(ASSIGN, NULL);

                // attach id to left node
                ast_add_node(assign, ast_node_init(ID, id.value));
                // attach expression to right node
                ast_add_node(assign, ast_node_init(EXPRESSION, expr.ast));
                return parser_result(assign);
            }
        }
        else
        {
            // ok it is not assignment, we must return back ID token
            parser_return_back(parser, id);
            return parser_result(NULL);
        }
    }
    else
    {
        // thats not assign statement
        return parser_result(NULL);
    }
}

parser_result_t small_statement(parser_t parser)
{
    if (accept_keyword(parser, "pass"))
    {
        return parser_result(ast_node_init(PASS, parser->previousToken.value));
    }
    else
    {
        return flow_statement(parser);
    }
}

parser_result_t simple_statement(parser_t parser)
{
    // try to find small statement
    parser_result_t small_stmt = small_statement(parser);

    if (small_stmt.error)
    {
        // just propagate error
        return small_stmt;
    }

    if (small_stmt.ast != NULL)
    {
        // we found small statement, now it must end with newline
        if (accept(parser, TNEWLINE))
        {
            return small_stmt;
        }
        else
        {
            // it does not end with endline which is syntax error
            // data holds literal of flow statement or pass
            return parser_error(small_stmt.ast, "Missing newline after symbol %s\n", small_stmt.ast->data);
        }
    }

    return small_stmt;
}

parser_result_t suite(parser_t parser)
{
    parser_result_t simple_stmt = simple_statement(parser);

    // if there is error, propagate it
    if (simple_stmt.error)
    {
        return simple_stmt;
    }

    // check if found simple statement, if we did so, return it
    if (simple_stmt.ast != NULL)
    {
        return simple_stmt;
    }

    if (accept(parser, TNEWLINE))
    {
        tToken eol = parser->previousToken;
        if (accept(parser, TINDENT))
        {
            ast_t consequent = ast_node_init(CONSEQUENT, NULL);

            for (ast_t stmt = statement(parser); stmt != NULL; stmt = statement(parser))
            {
                // todo: this is opportunity to generate code without building whole ast
                ast_add_node(consequent, stmt);
            }

            // empty suite, thats error
            if (queue_empty(consequent->nodes))
            {
                return parser_error(consequent, "Body requires at least one statement\n");
            }

            if (accept(parser, TDEDENT))
            {
                return parser_result(consequent);
            }
            else
            {
                // it was not followed with dedent, which is error
                return parser_error(consequent, "DEDENT syntax error\n");
            }
        }
        else
        {
            // we found newline, but it did not follow up by indent, so we dont know
            // even though, we better return it
            parser_return_back(parser, eol);
            return parser_result(NULL);
        }
    }
    else
    {
        // we dont know what it is
        return parser_result(NULL);
    }
}

parser_result_t if_and_elif(parser_t parser, char *keyword)
{
    // ensure we call it with if or elif only
    assert(strcmp(keyword, "if") == 0 || strcmp(keyword, "elif") == 0);

    if (accept_keyword(parser, keyword))
    {
        parser_result_t expr = parse_expression(parser);

        if (expr.error)
        {
            // propagate error
            return expr;
        }

        if (expr.ast == NULL)
        {
            return parser_error(NULL, "%s statement is missing condition\n", keyword);
        }

        if (accept(parser, TCOLON))
        {
            parser_result_t body = suite(parser);

            if (body.error)
            {
                // delete expression ast
                ast_delete(expr.ast);
                // propagate error
                return body;
            }

            if (body.ast == NULL)
            {
                return parser_error(NULL, "%s statement is missing body\n", keyword);
            }

            bool isIf = strcmp(keyword, "if") == 0;
            ast_t if_stmt = ast_node_init(isIf ? IF : ELIF, NULL);

            // lets connect nodes to if
            ast_add_node(if_stmt, expr.ast);
            ast_add_node(if_stmt, body.ast);

            return parser_result(if_stmt);
        }
        else
        {
            return parser_error(NULL, "%s statement is missing colon\n", keyword);
        }
    }
    else
    {
        // we dont know what it is
        return parser_result(NULL);
    }
}

// todo better error checking needed
parser_result_t if_statement(parser_t parser)
{
    parser_result_t if_stmt = if_and_elif(parser, "if");

    if (if_stmt.error || if_stmt.ast == NULL)
    {
        // propagate error or unknow structure
        return if_stmt;
    }

    parser_result_t previous = if_stmt;

    // lets iterate over elif conditions
    for (
        parser_result_t elif_stmt = if_and_elif(parser, "elif");
        elif_stmt.ast != NULL && elif_stmt.error == NULL;
        elif_stmt = if_and_elif(parser, "elif"))
    {
        // add alternative branch to if, aka elif
        // body and condition were added in if_and_elif
        ast_add_node(previous.ast, elif_stmt.ast);
        previous = elif_stmt;
    }

    // lets check else
    if (accept_keyword(parser, "else"))
    {
        if (accept(parser, TCOLON))
        {
            // try to get body of else
            parser_result_t body = suite(parser);

            if (body.error)
            {
                // propagate error
                return body;
            }

            if (body.ast == NULL)
            {
                return parser_error(NULL, "else statement is missing body\n");
            }

            ast_t else_stmt = ast_node_init(ELSE, NULL);

            // lets connect body to else
            ast_add_node(else_stmt, body.ast);

            return parser_result(else_stmt);
        }
        else
        {
            return parser_error(if_stmt.ast, "Missing color after else condition");
        }
    }
    else
    {
        return if_stmt;
    }
}

parser_result_t while_statement(parser_t parser)
{
    if (accept_keyword(parser, "while"))
    {
        {
            ast_t while_stmt = ast_node_init(WHILE, NULL);
            parser_result_t comp = parse_expression(parser);

            // check condition for error
            if (comp.error)
            {
                ast_delete(while_stmt);
                return comp;
            }

            if (comp.ast == NULL)
            {
                return parser_error(while_stmt, "While statement is missing condition\n");
            }
            // end check condition for error

            ast_add_node(while_stmt, comp.ast);

            // check suite for error
            parser_result_t suites = suite(parser);

            if (suites.error)
            {
                ast_delete(while_stmt);
                return suites;
            }

            if (suites.ast == NULL)
            {
                return parser_error(while_stmt, "While statement is missing body\n");
            }
            // end check suite for error

            ast_add_node(while_stmt, suites.ast);

            if (accept_keyword(parser, "else"))
            {
                if (accept(parser, TCOLON))
                {
                    if (suites.error)
                    {
                        ast_delete(while_stmt);
                        return suites;
                    }

                    if (suites.ast == NULL)
                    {
                        return parser_error(while_stmt, "Else after While statement is missing body\n");
                    }

                    // add alternate node
                    ast_add_node(while_stmt, suites.ast);

                    return parser_result(while_stmt);
                }
                else
                {
                    return parser_error(while_stmt, "Else after While statement is missing colon\n");
                }
            }
        }
    }
}

parser_result_t try_statements(parser_t parser, parser_method_t methods[], size_t method_size)
{
    for (size_t i = 0; i < method_size; i++)
    {
        // try every statement
        parser_result_t stmt = methods[i](parser);

        if (stmt.error)
        {
            // propagate errors
            return stmt;
        }

        if (stmt.ast != NULL)
        {
            // we found statement, returning it
            return stmt;
        }
    }
    // no statement found
    return parser_result(NULL);
}

parser_result_t compound_statement(parser_t parser)
{
    parser_method_t methods[] = {if_statement, while_statement, functionDef};

    return try_statements(parser, methods, 3);
}

parser_result_t statement(parser_t parser)
{
    parser_method_t methods[] = {simple_statement, compound_statement};
    return try_statements(parser, methods, 2);
}

ast_t parse()
{
    parser_t parser = parser_init();
    parser_next(parser);

    // todo: parsing

    parser_destroy(parser);
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
ast_t functionDef(parser_t parser, queue_t queue)
{
    ast_t ast = ast_node_init_empty();
    if (accept(parser, TKEYWORD))
    {
        if (ptr_string_c_equals(parser->previousToken.value, "def"))
        {
            stderr_print(line, TKEYWORD, parser);
            return NULL;
        }

        if (accept(parser, TIDENTIFICATOR))
        {
            ast_t parent = ast_node_init(parser->previousToken);
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
