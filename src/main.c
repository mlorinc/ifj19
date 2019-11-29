#include <stdio.h>
#include "array_nodes.c"
#include "deque.c"
#include "error.c"
#include "expression_parser.c"
#include "iterator.c"
#include "parser_ast.c"
#include "parser_result.c"
#include "parser.c"
#include "ptr_string.c"
#include "queue.c"
#include "scaner.c"
//#include "semantic_analyser.c"
#include "stack.c"
#include "symtable.c"
#include "array_nodes.h"
#include "deque.h"
#include "error.h"
#include "expression_parser.h"
#include "iterator.h"
#include "parser_ast.h"
#include "parser_result.h"
#include "parser.h"
#include "ptr_string.h"
#include "queue.h"
#include "scaner.h"
//#include "semantic_analyser.h"
#include "stack.h"
#include "symtable.h"
//#include "expression_parser.h"
//#include "ptr_string.h"
//#include "queue.h"


int main(void)
{
    parser_t parse = parser_init();
    tToken *token;
    char * string;
    parse->returned_tokens = queue_init();

    parse_expression(parse);

    for (size_t i = 0; i < deque_size(parse->returned_tokens); i++)
    {
        token = queue_pop(parse->returned_tokens);
        if (token->value != NULL)
        {
            string = ptr_string_c_string(token->value);
            printf("Token VALUE: %s\n", string);
        }
        printf("Token TYPE: %d\n", token->type);
        
    }
    printf("KONEC\n");
    

    return 0;
}