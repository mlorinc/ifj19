/**
 * IFJ2019 @ VUT FIT Brno
 * 09.11.2019
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
 * File: generator.h
 *
 */
#ifndef IFJ19_GENERATOR_H
#define IFJ19_GENERATOR_H

#include "semantic_analyser.c"
#include "generator.h"
#include "parser_ast.c"
#include "array_nodes.h"

error_codes generate(ast_t ast, scope_t scope);

void generate_start();

void generate_function_definition(ast_t ast, scope_t scope);

size_t get_number_of_parameters(ast_t ast);

void generate_label(ast_t ast, unsigned line);

void generate_defvar_of_params(ast_t ast, size_t numberOfParams);

void generate_define_frame_and_retvalue();

ast_t get_values_of_params(ast_t ast);

void generate_end_of_function(ast_t ast);

#endif //IFJ19_GENERATOR_H
