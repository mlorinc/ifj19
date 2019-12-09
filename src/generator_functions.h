#ifndef __H_GENERATOR_FUNCTIONS
#define __H_GENERATOR_FUNCTIONS

#include "queue.h"

/**
 * Generates ifj19 code for if/elif condition
 * @param expression condition. If label name is "else", expression is set to NULL
 * @param label_name name of label "if" | "elif" | "else" | "ENDIF (if else is missing)"
 * @param line line of if or elif
 */ 
void generate_condition(queue_t expression, char *label_name, unsigned line);

/**
 * Generates ENDIF label for if elif else sequence
 * LABEL ENDIF$line
 * @param line 
 */ 
void generate_endif(unsigned line);

/**
 * Generates ENDIF jump for if elif else. It is used as last command to get out of if conditions.
 * jmp ENDIF$line
 * @param line 
 */ 
void generate_endif_jump(unsigned line);

/**
 * Generates return. If expression is NULL, it means it returns None
 */ 
void generate_return(queue_t expression);

#endif
