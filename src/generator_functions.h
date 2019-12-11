#ifndef __H_GENERATOR_FUNCTIONS
#define __H_GENERATOR_FUNCTIONS

#include "queue.h"
#include "scope.h"
#include <stdio.h>

/**
 * Generates ifj19 code for if/elif condition
 * @param expression condition. If label name is "else", expression is set to NULL
 * @param label_name name of label "if" | "elif" | "else" | "ENDIF (if else is missing)"
 * @param line line of if or elif
 */ 
void generate_condition(scope_t scope, queue_t expression, char *label_name, size_t line);

/**
 * Generates ENDIF label for if elif else sequence
 * LABEL ENDIF$line
 * @param line 
 */ 
void generate_endif(size_t line);

/**
 * Generates ENDIF jump for if elif else. It is used as last command to get out of if conditions.
 * jmp ENDIF$line
 * @param line 
 */ 
void generate_endif_jump(size_t line);

/**
 * Generates return. If expression is NULL, it means it returns None
 */ 
void generate_return(scope_t scope, queue_t expression);

/**
 * Generates header of function. params is never NULL.
 */ 
void generate_function_header(char *fun_name, array_nodes_t params);

/**
 * Generates code before function returns
 */ 
void generate_function_footer();

/**
 * Generated code which calls function
 * @param node function call node
 * @param current_scope scope, where call is performed. Use it do deduce if it should use local or global variable
 */ 
void generate_function_call(scope_t current_scope, ast_t func_call);

/**
 * Generates code which stores return value of function to id
 */ 
void generate_function_call_assignment(scope_t current_scope, const char *id, ast_t func_call);

/**
 * Generates code which stores expression value to id
 */ 
void generate_expression_assignment(scope_t current_scope, const char *id, ast_t expression);

/**
 * Generates while label
 * WHILE$line
 */ 
void generate_while_label(size_t line);

/**
 * Generate while condition
 */ 
void generate_while_condition(scope_t scope, queue_t expression);

/**
 * Generate WHILEELSE$line label
 */ 
void generate_while_else_label(size_t line);

/**
 * Generate jump to else structure if condition was false
 */ 
void generate_while_jump_to_else_if_false(size_t line);

/**
 * Jump out of while if condition is false
 */ 
void generate_while_jump_to_endwhile_if_false(size_t line);

/**
 * Generate jump to WHILE$line
 */ 
void generate_while_jump(size_t line);

/**
 * Generate jump to ENDWHILE$line
 */ 
void generate_endwhile_jump(size_t line);

/**
 * Generates end while label
 * ENDWHILE$line
 */ 
void generate_endwhile(size_t line);

void generate_semantic_check_add();

void generate_semantic_check_sub();

void generate_semantic_check_mul();

void generate_semantic_check_div();

void generate_semantic_check_idiv();
void generate_print(scope_t scope, array_nodes_t params);

#endif
