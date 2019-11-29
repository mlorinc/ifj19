#ifndef __H_SEMANTIC_ANALYSER
#define __H_SEMANTIC_ANALYSER

#include "parser_ast.h"
#include "queue.h"

typedef queue_t semantic_errors_t;

void semantic_analysis(ast_t root);

#endif 
