#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include "parser.h"
#include "queue.h"
#include "ptr_string.h"
#include "parser_result.h"
#include "expression_parser.h"

typedef parser_result_t (*parser_method_t)(parser_t parser);

parser_result_t suite(parser_t parser);
parser_result_t function_call(parser_t parser);

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
    if (parser->token.type == TKEYWORD && ptr_string_c_equals(parser->token.value, keyword))
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
        return parser_result(ast_node_init(BREAK, parser->previousToken.line, parser->previousToken.pos, parser->previousToken.value));
    }
    else if (accept_keyword(parser, "continue"))
    {
        return parser_result(ast_node_init(CONTINUE, parser->previousToken.line, parser->previousToken.pos, parser->previousToken.value));
    }
    else if (accept_keyword(parser, "return"))
    {
        unsigned line = parser->previousToken.line;
        unsigned pos = parser->previousToken.pos;
        // create return node
        ast_t ret = ast_node_init(RETURN, line, pos, parser->previousToken.value);

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
            ast_add_node(ret, ast_node_init(NONE, line, pos, NULL));
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
    tToken prevToken = parser->previousToken;
    if (accept(parser, TIDENTIFICATOR))
    {
        tToken id = parser->previousToken;
        if (accept(parser, TASSIGN))
        {
            unsigned assign_line = parser->previousToken.line;
            unsigned assign_pos = parser->previousToken.pos;
            // we are ok, lets create tree
            ast_t assign = ast_node_init(ASSIGN, assign_line, assign_pos, NULL);

            // attach id to left node
            ast_add_node(assign, ast_node_init(ID, id.line, id.pos, id.value));
            // try to get function call
            parser_result_t fnc_call = function_call(parser);

            if (fnc_call.error)
            {
                return fnc_call;
            }

            if (fnc_call.ast != NULL)
            {
                ast_add_node(assign, fnc_call.ast);
                return parser_result(assign);
            }

            // try to get expression
            parser_result_t expr = parse_expression(parser);

            // check if error occurred
            if (expr.error)
            {
                // something bad happened, we dont want to return back token,
                // we have to continue with parsing
                return expr;
            }

            if (expr.ast != NULL)
            {
                ast_add_node(assign, expr.ast);
                return parser_result(assign);
            }

            return parser_error(NULL, "Invalid assignment, missing expression or function call on right hand side (line %u)\n", assign_line);
        }
        else
        {
            // ok it is not assignment, we must return back ID token
            parser_return_back(parser, parser->token);
            parser->previousToken = prevToken;
            parser->token = id;
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
    while (accept(parser, TNEWLINE))
    {
        /* just skip eol */
    }

    if (accept_keyword(parser, "pass"))
    {
        return parser_result(ast_node_init(PASS, parser->previousToken.line, parser->previousToken.pos, parser->previousToken.value));
    }

    parser_result_t flow_stmt = flow_statement(parser);

    if (flow_stmt.ast != NULL || flow_stmt.error)
    {
        return flow_stmt;
    }

    parser_result_t assign = assign_statemnt(parser);

    if (assign.ast != NULL || assign.error)
    {
        return assign;
    }

    parser_result_t func_call = function_call(parser);

    if (func_call.ast != NULL || func_call.error)
    {
        return func_call;
    }

    parser_result_t expr = parse_expression(parser);

    if (expr.ast != NULL || expr.error)
    {
        return expr;
    }

    return parser_result(NULL);
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
            return parser_error(small_stmt.ast, "Missing newline on line %u\n", small_stmt.ast->line);
        }
    }

    return small_stmt;
}

parser_result_t if_and_elif(parser_t parser, char *keyword)
{
    // ensure we call it with if or elif only
    assert(strcmp(keyword, "if") == 0 || strcmp(keyword, "elif") == 0);

    if (accept_keyword(parser, keyword))
    {
        tToken if_token = parser->previousToken;
        parser_result_t expr = parse_expression(parser);

        if (expr.error)
        {
            // propagate error
            return expr;
        }

        if (expr.ast == NULL)
        {
            return parser_error(NULL, "%s statement is missing condition (line %u)\n", keyword, if_token.line);
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
                return parser_error(NULL, "%s statement is missing body (line %u)\n", keyword, if_token.line);
            }

            bool isIf = strcmp(keyword, "if") == 0;
            ast_t if_stmt = ast_node_init(isIf ? IF : ELIF, if_token.line, if_token.pos, NULL);

            // lets connect nodes to if
            ast_add_node(if_stmt, expr.ast);
            ast_add_node(if_stmt, body.ast);

            return parser_result(if_stmt);
        }
        else
        {
            return parser_error(NULL, "%s statement is missing colon (line %u)\n", keyword, if_token.line);
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
        tToken else_token = parser->previousToken;
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
                return parser_error(NULL, "else statement is missing body (line %u)\n", else_token.line);
            }

            ast_t else_stmt = ast_node_init(ELSE, else_token.line, else_token.pos, NULL);

            // lets connect body to else
            ast_add_node(else_stmt, body.ast);
            return parser_result(else_stmt);
        }
        else
        {
            return parser_error(if_stmt.ast, "Missing color after else condition (line %u)\n", else_token.line);
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
        ast_t while_stmt = ast_node_init(WHILE, parser->previousToken.line, parser->previousToken.pos, NULL);
        parser_result_t comp = parse_expression(parser);

        // check condition for error
        if (comp.error)
        {
            ast_delete(while_stmt);
            return comp;
        }

        if (comp.ast == NULL)
        {
            return parser_error(while_stmt, "while statement is missing condition (line %u)\n", while_stmt->line);
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
            return parser_error(while_stmt, "while statement is missing body (line %u)\n", while_stmt->line);
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
                    return parser_error(while_stmt, "else after while statement is missing body (line %u)\n", while_stmt->line);
                }

                // add alternate node
                ast_add_node(while_stmt, suites.ast);

                return parser_result(while_stmt);
            }
            else
            {
                return parser_error(while_stmt, "else after while statement is missing colon (line %u)\n", while_stmt->line);
            }
        }
        else
        {
            return parser_result(while_stmt);
        }
    }
    else
    {
        // this is not while statement
        return parser_result(NULL);
    }
}

parser_result_t function_definition_parameter(parser_t parser)
{
    if (accept(parser, TIDENTIFICATOR))
    {
        return parser_result(ast_node_init(ID, parser->previousToken.line, parser->previousToken.pos, parser->previousToken.value));
    }
    else
    {
        return parser_result(NULL);
    }
}

parser_result_t function_call_parameter(parser_t parser)
{
    if (accept(parser, TIDENTIFICATOR) || accept(parser, TSTRING) || accept(parser, TINT) || accept(parser, TFLOAT))
    {
        ast_node_type_t argument_type;

        switch (parser->previousToken.type)
        {
        case TIDENTIFICATOR:
            argument_type = ID;
            break;
        case TSTRING:
            argument_type = STRING_LITERAL;
            break;
        case TINT:
            argument_type = INT_LITERAL;
            break;
        case TFLOAT:
            argument_type = FLOAT_LITERAL;
            break;
        default:
            return parser_error(NULL, "%s:%d: INTERNAL ERROR\n", __FILE__, __LINE__);
        }

        return parser_result(ast_node_init(argument_type, parser->previousToken.line, parser->previousToken.pos, parser->previousToken.value));
    }
    else
    {
        return parser_result(NULL);
    }
}

parser_result_t function_paramaters(parser_t parser, parser_method_t get_parameter)
{
    // function parameter list must begin with (
    if (!accept(parser, TLEFTPAR))
    {
        return parser_error(NULL, "expecting left parenthesis after function name (line %u)\n", parser->previousToken.line);
    }

    parser_result_t param = get_parameter(parser);
    ast_t parameters = ast_node_init(FUNCTION_PARAMETERS, 0, 0, NULL);

    ast_add_node(parameters, param.ast);

    while (param.ast != NULL)
    {
        if (accept(parser, TCOMMA))
        {
            param = get_parameter(parser);
            if (param.ast == NULL)
            {
                return parser_error(parameters, "invalid trailing comma (line %u)\n", parser->previousToken.line);
            }
            else
            {
                ast_add_node(parameters, param.ast);
            }
        }
        else
        {
            break;
        }
    }

    // function parameter list must end with )
    if (!accept(parser, TRIGHTPAR))
    {
        return parser_error(parameters, "expecting right parentthesis after function parameters (line %u)\n", parser->previousToken.line);
    }

    return parser_result(parameters);
}

parser_result_t function_body_suite(parser_t parser)
{
    return suite(parser);
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
parser_result_t function_def(parser_t parser)
{
    if (accept_keyword(parser, "def"))
    {
        if (accept(parser, TIDENTIFICATOR))
        {
            // initialize function definition node with name of function
            ast_t fn_def = ast_node_init(FUNCTION_DEFINITION, parser->previousToken.line, parser->previousToken.pos, parser->previousToken.value);
            // get parameters of function
            parser_result_t parameters = function_paramaters(parser, function_definition_parameter);
            // check errors
            if (parameters.error)
            {
                ast_delete(fn_def);
                return parameters;
            }

            if (accept(parser, TCOLON))
            {
                // get body of function, its ast must not be null and must not error
                parser_result_t function_body_suites = function_body_suite(parser);

                ast_add_node(fn_def, parameters.ast);
                ast_add_node(fn_def, function_body_suites.ast);

                if (function_body_suites.error)
                {
                    return parser_error_ptr_string(fn_def, function_body_suites.error);
                }
                else if (function_body_suites.ast == NULL)
                {
                    return parser_error(fn_def, "expecting function body after definition (line %u)\n", fn_def->line);
                }
                else
                {
                    return parser_result(fn_def);
                }
            }
            else
            {
                return parser_error(fn_def, "expecting colon after argument list (line %u)\n", fn_def->line);
            }
        }
        else
        {
            return parser_error(NULL, "expecting function name after def (line %u)\n", parser->previousToken.line);
        }
    }
    else
    {
        // not function definition
        return parser_result(NULL);
    }
}

parser_result_t function_call(parser_t parser)
{
    tToken prevToken = parser->previousToken;
    if (accept(parser, TIDENTIFICATOR))
    {
        tToken id = parser->previousToken;
        // check if this is function call
        if (parser->token.type != TLEFTPAR)
        {
            // not function call
            parser_return_back(parser, parser->token);
            parser->previousToken = prevToken;
            parser->token = id;
            return parser_result(NULL);
        }

        parser_result_t params = function_paramaters(parser, function_call_parameter);

        if (params.error)
        {
            return params;
        }

        ast_t func_call = ast_node_init(FUNCTION_CALL, params.ast->line, params.ast->pos, id.value);
        ast_add_node(func_call, params.ast);
        return parser_result(func_call);
    }
    else
    {
        return parser_result(NULL);
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
    parser_method_t methods[] = {if_statement, while_statement, function_def};

    return try_statements(parser, methods, 3);
}

parser_result_t statement(parser_t parser)
{
    parser_method_t methods[] = {simple_statement, compound_statement};
    return try_statements(parser, methods, 2);
}

parser_result_t suite(parser_t parser)
{
    tToken previousToken = parser->previousToken;
    if (accept(parser, TNEWLINE))
    {
        tToken eol = parser->previousToken;
        if (accept(parser, TINDENT))
        {
            ast_t consequent = ast_node_init(CONSEQUENT, 0, 0, NULL);

            for (parser_result_t stmt = statement(parser); stmt.ast != NULL || stmt.error != NULL; stmt = statement(parser))
            {
                if (stmt.error)
                {
                    ast_delete(consequent);
                    return stmt;
                }
                ast_add_node(consequent, stmt.ast);
            }

            // empty suite, thats error
            if (array_nodes_empty(consequent->nodes))
            {
                return parser_error(consequent, "body requires at least one statement (line %u)\n", eol.line + 1);
            }

            if (accept(parser, TDEDENT))
            {
                return parser_result(consequent);
            }
            else
            {
                // it was not followed with dedent, which is error
                return parser_error(consequent, "DEDENT syntax error (line %u)\n", eol.line + 1);
            }
        }
        else
        {
            // we found newline, but it did not follow up by indent, so we dont know
            // even though, we better return it
            parser->previousToken = previousToken;
            parser->token = eol;
            return parser_result(NULL);
        }
    }
    else
    {
        // we dont know what it is
        return parser_result(NULL);
    }
}

parser_result_t parse()
{
    parser_t parser = parser_init();
    parser_next(parser);

    ast_t program = ast_node_init(CONSEQUENT, 0, 0, NULL);

    enum error_codes status = ERROR_OK;

    for (parser_result_t stmt = statement(parser);
         stmt.ast || stmt.error || parser->token.type != TENDOFFILE;
         stmt = statement(parser))
    {
        if (parser->token.type == TLEXERR)
        {
            status = ERROR_LEX;
            break;
        }
        else if (parser->token.type == TERR)
        {
            status = ERROR_INTERNAL;
            break;
        }
        else if (stmt.error)
        {
            status = ERROR_SYNTAX;
            char *err = ptr_string_c_string(stmt.error);
            fprintf(stderr, "[PARSER]: %s\n", err);
            free(err);
            parser_error_dispose(stmt);
            break;
        }
        else if (stmt.ast == NULL)
        {
            // error occurred before, and it left unprocessed tokens
            status = ERROR_INTERNAL;
            fprintf(stderr, "%s:%d: Unexpected token type: %d, line: %u\n", __FILE__, __LINE__, parser->token.type, parser->token.line);
            break;
        }
        else
        {
            ast_add_node(program, stmt.ast);
        }
    }

    parser_destroy(parser);

    parser_result_t result = parser_result(program);
    result.error_code = status;

    return result;
}
