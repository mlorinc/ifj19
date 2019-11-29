#include <stdio.h>
#include "expression_parser.h"
#include "ptr_string.h"
#include "queue.h"

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