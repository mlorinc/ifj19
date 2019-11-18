//
// Created by bixor on 11/18/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef enum {
    TKEYWORD,
    TIDENTIFIER,
    TLEFTPAR,
    TCOMMA,
    TRIGHTPAR,
    TCOLON,
    TNEWLINE,
    TENDOFFILE
} tToken_type;

typedef struct {
    char *value;
    tToken_type type;
} tToken;

typedef struct parser
{
    tToken token;
    tToken previousToken;
} * parser_t;

parser_t parser_init()
{
    return malloc(sizeof(struct parser));
}

void parser_destroy(parser_t parser)
{
    assert(parser != NULL);
    free(parser);
}

struct AST
{
    struct AST *node;
    tToken token;
};

typedef struct AST *AST_T;
typedef struct AST **AST_ARRAY;

void parser_next(parser_t parser, tToken token);
AST_T node_init_empty();
AST_T build_param_tree(parser_t parser);

int main()
{
    parser_t parser = parser_init();

    /**
    * *************************
    * START OF LEXER SIMULATION
    * START OF LEXER SIMULATION
    * START OF LEXER SIMULATION
    * *************************
    */
    tToken token1;
    tToken token2;
    tToken token3;
    tToken token4;
    tToken token5;
    tToken token6;
    tToken token7;
    tToken token8;
    tToken token9;

    token1.value = "def";
    token1.type = TKEYWORD;
    token2.value = "foo";
    token2.type = TIDENTIFIER;
    token3.value = "(";
    token3.type = TLEFTPAR;
    token4.value = "a";
    token4.type = TIDENTIFIER;
    token5.value = ",";
    token5.type = TCOMMA;
    token6.value = "b";
    token6.type = TIDENTIFIER;
    token7.value = ")";
    token7.type = TRIGHTPAR;
    token8.value = ":";
    token8.type = TCOLON;
    token9.type = TNEWLINE;
    /**
    * ***********************
    * END OF LEXER SIMULATION
    * END OF LEXER SIMULATION
    * END OF LEXER SIMULATION
    * ***********************
    */


    /**
     * ********************
     * START OF PARSER PART
     * START OF PARSER PART
     * START OF PARSER PART
     * ********************
     */

    AST_T ast;

    parser_next(parser, token1);
    if (parser->token.type == TKEYWORD){
        if (!strcmp(parser->token.value, "def")){

            parser_next(parser, token2);
            if (parser->token.type == TIDENTIFIER){
                ast = node_init_empty();
                ast->token = parser->token;

                parser_next(parser, token3);
                if(parser->token.type == TLEFTPAR){

                    parser_next(parser, token4);
                    if (parser->token.type == TIDENTIFIER){ //now we know that we got params in function
                        //CALL PARAM FUNCTION TO SET PARAMS
                        AST_T ast = node_init_empty();
                        AST_ARRAY ast_arr = malloc(sizeof(AST_ARRAY));

                        /**
                         * SIMULATION OF WHILE
                         */
                        /*while (parser->token.type != TRIGHTPAR) {
                            if (parser->token.type == TCOMMA) {

                            }
                        }*/

                        if (parser->token.type == TIDENTIFIER){
                            ast_arr[0] = node_init_empty();
                            ast_arr[0]->token.type = TIDENTIFIER;
                            ast_arr[0]->token.value = parser->token.value;
                        }

                        parser_next(parser, token5);
                        if (parser->token.type == TCOMMA){
                        }
                        else if (parser->token.type == TRIGHTPAR){
                            return 0;
                        }

                        parser_next(parser, token6);
                        if (parser->token.type == TIDENTIFIER){
                            ast_arr = realloc(ast_arr, 2*sizeof(AST_ARRAY));
                            ast_arr[1] = node_init_empty();
                            ast_arr[1]->token.type = TIDENTIFIER;
                            ast_arr[1]->token.value = parser->token.value;
                        }

                        parser_next(parser, token7);
                        if (parser->token.type == TRIGHTPAR){
                            ast->node = ast_arr;
                        }
                    }
                }
            }
        }
    }
    /**
     * ******************
     * END OF PARSER PART
     * END OF PARSER PART
     * END OF PARSER PART
     * ******************
     */

    printf("%u %s\n", ast->token.type, ast->token.value);
    printf("%u %s\n", ast->node->token.type, ast->node->token.value);

    parser_destroy(parser);
    return 0;
}

void parser_next(parser_t parser, tToken token)
{
    assert(parser != NULL);
    parser->previousToken = parser->token;
    parser->token = token;
}

AST_T node_init_empty()
{
    return malloc(sizeof(struct AST));
}

/*AST_T build_param_tree(parser_t parser)
{
    /*AST_T ast = node_init_empty();
    AST_ARRAY ast_arr = malloc(sizeof(AST_ARRAY));*/

    /**
     * SIMULATION OF WHILE
     */
    /*while (parser->token.type != TRIGHTPAR) {
        if (parser->token.type == TCOMMA) {

        }
    }

    if (parser->token.type == TIDENTIFIER){
        ast_arr[0] = node_init_empty();
        ast_arr[0]->token.type = TIDENTIFIER;
        ast_arr[0]->token.value = parser->token.value;
    }

    parser_next(parser, token5);
    if (parser->token.type == TCOMMA){
    }
    else if (parser->token.type == TRIGHTPAR){
        return ast;
    }

    parser_next(parser, token6);
    if (parser->token.type == TIDENTIFIER){
        ast_arr = realloc(ast_arr, 2*sizeof(AST_ARRAY));
        ast_arr[1] = node_init_empty();
        ast_arr[1]->token.type = TIDENTIFIER;
        ast_arr[1]->token.value = parser->token.value;
    }

    parser_next(parser, token7);
    if (parser->token.type == TRIGHTPAR){
        ast->node = &ast_arr;
        return ast;
    }
    else {
        return NULL;
    }
}*/