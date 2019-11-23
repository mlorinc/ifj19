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
    struct AST **node;
    tToken token;
};

typedef struct AST *AST_T;
typedef struct AST **AST_ARR;

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
    * simulation of: def foo(a,b):
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
    AST_ARR toDelete; //to free at the end of function

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
                        AST_ARR params = malloc(sizeof(AST_ARR));

                        /**
                         * SIMULATION OF WHILE
                         */
                        /*while (parser->token.type != TRIGHTPAR) {
                            if (parser->token.type == TCOMMA) {

                            }
                        }*/

                        if (parser->token.type == TIDENTIFIER){
                            params[0] = node_init_empty();
                            params[0]->token.type = TIDENTIFIER;
                            params[0]->token.value = parser->token.value;
                        }

                        parser_next(parser, token5);
                        if (parser->token.type == TCOMMA){
                        }
                        else if (parser->token.type == TRIGHTPAR){
                            return 0;
                        }

                        parser_next(parser, token6);
                        if (parser->token.type == TIDENTIFIER){
                            params = realloc(params, 2*sizeof(AST_ARR));
                            params[1] = node_init_empty();
                            params[1]->token.type = TIDENTIFIER;
                            params[1]->token.value = parser->token.value;
                        }

                        parser_next(parser, token7);
                        if (parser->token.type == TRIGHTPAR){
                            ast->node = params; //KVOLI TOMUTO MUSI BYT **node aby sme tam vopchali potom cely ten param strom / hocijaky vacsi rozvetveny
                            toDelete = params;
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
    printf("%u %s\n", ast->node[0]->token.type, ast->node[0]->token.value);
    printf("%u %s\n", ast->node[1]->token.type, ast->node[1]->token.value);

    for (int i = 0; i < 2; i++){
        free(ast->node[i]);
    }

    free(ast);
    free(toDelete);
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
