/**
 * IFJ2019 @ VUT FIT Brno
 * 30.11.2019
 *
 * Variant 2
 * Team: 054
 *
 * Authors:
 * Marian Lorinc (xlorin01)
 * Lukas Javorsky (xjavor20)
 * Patrik Ondriga (xondri08)
 * Peter Vinarcik (xvinar00)
 *
 * File: expression_parser.h
 *
 */

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
