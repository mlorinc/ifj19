#ifndef __H_PARSER
#define __H_PARSER

struct AST
{
    struct AST *left;
    struct AST *right;
    tToken token;
};

typedef struct AST *AST_T; 

AST_T parse();


#endif
