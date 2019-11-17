#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "parser.h"

AST_T node_init_empty() {
    return malloc(sizeof(struct AST));
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
    parser->token = getToken();
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

AST_T term(parser_t parser) {
    AST_T f1 = factor(parser);
    if (f1 != NULL) {
        AST_T last_ast = node_init_empty();
        AST_T current_ast = node_init_empty();
        current_ast->left = f1;
        for (;;)
        {
            if (accept(parser, TMUL) || accept(parser, TDIV) || accept(parser, TMOD))
            {
                current_ast->token = parser->previousToken;

                AST_T f2 = factor(parser);
                if (f2 != NULL) {
                    current_ast->right = f2;
                }
                else {
                    return NULL;
                }
                
                last_ast = current_ast;
                current_ast = node_init_empty();

                current_ast->left = last_ast;
            }
            else {
                return current_ast;
            }
        }
        
    }
    else {
        return NULL;
    }
}

AST_T expression(parser_t parser)
{
    if (accept(parser, TFLOAT) || accept(parser, TINT))
    {
        return node_init(parser->previousToken);
    }
    if (accept(parser, TLEFTPAR))
    {
        AST_T expr = expression(parser);
        if (expr != NULL)
        {
            if (accept(parser, TRIGHTPAR))
            {
                return expr;
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
    return NULL;
}

AST_T parse()
{
    parser_t parser = parser_init();
    parser_next(parser);

    //function parse
    if (parser->token.type == THEADER){
        //TODO function to parse header
        //where the code begins
    }
    if (parser->token.type == TINDENT){
        //TODO function to parse indent
    }
    if (parser->token.type == TDEDENT){
        //TODO function to parse dedent
    }
    if (parser->token.type == TINT){
        //TODO function to parse int
    }
    if (parser->token.type == TFLOAT){
        //TODO function to parse float
    }
    if (parser->token.type == TSTRING){
        //TODO function to parse string
    }
    if (parser->token.type == TKEYWORD){
        //TODO function to parse keyword
        //here we must identify, which keyword we got
        //f.e -> if, pass, while, break, ...
        if (!strcmp(&(*(char*)parser->token.value), "if")){

        }
        if (!strcmp(&(*(char*)parser->token.value), "else")){

        }
        if (!strcmp(&(*(char*)parser->token.value), "while")){

        }
        if (!strcmp(&(*(char*)parser->token.value), "pass")){

        }
        if (!strcmp(&(*(char*)parser->token.value), "return")){

        }
    }
    if (parser->token.type == TIDENTIFICATOR){
        //TODO function to parse identificator
    }
    if (parser->token.type == TADD){
        //TODO function to parse add
    }
    if (parser->token.type == TSUB){
        //TODO function to parse sub
    }
    if (parser->token.type == TMUL){
        //TODO function to parse mul
    }
    if (parser->token.type == TDIV){
        //TODO function to parse div
    }
    if (parser->token.type == TMOD){
        //TODO function to parse mod
    }

    /**
     * FROM THIS POINT IM NOT SURE IF THIS IS NECESSARY
     * TO HAVE THIS IN PARSE FUNCTION CAUSE WE EXPECT OTHERS
     * TOKENS FIRST
     *
     * TOMORROW WE HAVE TO DISCUSS THIS
     */
    if (parser->token.type == TLT){
        //TODO function to parse less than
    }
    if (parser->token.type == TGT){
        //TODO function to parse greater than
    }
    if (parser->token.type == TLTE){
        //TODO function to parse <=
    }
    if (parser->token.type == TGTE){
        //TODO function to parse >=
    }
    if (parser->token.type == TEQ){
        //TODO function to parse ==
    }
    if (parser->token.type == TNE){
        //TODO function to parse !=
    }
    if (parser->token.type == TLEFTPAR){
        //TODO function to parse (
    }
    if (parser->token.type == TRIGHTPAR){
        //TODO function to parse )
    }
    /**
     * END OF SECTION
     * MENTIONED UPWARDS
     */

    parser_destroy(parser);
}


