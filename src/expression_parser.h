#ifndef __H_EXPRESSION_PARSER
#define __H_EXPRESSION_PARSER

#include "parser_result.h"
#include "parser.h"

/**
 * Parses expression and adds elements to queue in polish notation
 * @param parser parser object
 * @return postfix expression or NULL if parse error occurred
 */ 
parser_result_t parse_expression(parser_t parser);

#endif 
