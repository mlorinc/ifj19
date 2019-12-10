#include "queue.h"
#include "generator_functions.h"
#include "deque.c"
#include "scope.c"

/**
 * Generates ifj19 code for if/elif condition
 * @param expression condition. If label name is "else", expression is set to NULL
 * @param label_name name of label "if" | "elif" | "else" | "ENDIF (if else is missing)"
 * @param line line of if or elif
 */
void generate_condition(scope_t scope, queue_t expression, char *label_name, unsigned line)
{
    if (get_function_scope(scope) == NULL) { //we are in GLOBAL FRAME
        if (!strcmp(expression->last, '>')) {
            printf("GT cond%u GF@%s GF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("JUMPIFEQ %s%u bool@true bool@cond%u\n", label_name, line, line);
        }
        else if (!strcmp(expression->last, '<')) {
            printf("LT cond%u GF@%s GF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("JUMPIFEQ %s%u bool@true bool@cond%u\n", label_name, line, line);
        }
        else if (!strcmp(expression->last, '==')) {
            printf("JUMPIFEQ %s%u GF@%s GF@%s\n", label_name, line, (char *) expression->first, (char *) expression->first->next);
        }

        //TODO <= >=
    }

    else { //we are in LOCAL FRAME
        if (!strcmp(expression->last, '>')) {
            printf("GT cond%u LF@%s LF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("JUMPIFEQ %s%u bool@true bool@cond%u\n", label_name, line, line);
        }
        else if (!strcmp(expression->last, '<')) {
            printf("LT cond%u LF@%s LF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("JUMPIFEQ %s%u bool@true bool@cond%u\n", label_name, line, line);
        }
        else if (!strcmp(expression->last, '==')) {
            printf("JUMPIFEQ %s%u LF@%s LF@%s\n", label_name, line, (char *) expression->first, (char *) expression->first->next);
        }

        //TODO <= >=
    }

}

/**
 * Generates ENDIF label for if elif else sequence
 * LABEL ENDIF$line
 * @param line
 */
void generate_endif(unsigned line)
{
    printf("LABEL ENDIF$%u\n", line);
}

/**
 * Generates ENDIF jump for if elif else. It is used as last command to get out of if conditions.
 * jmp ENDIF$line
 * @param line
 */
void generate_endif_jump(unsigned line)
{
    printf("JUMP ENDIF$%u\n", line);
}

/**
 * Generates return. If expression is NULL, it means it returns None
 */
void generate_return(scope_t scope, queue_t expression)
{
    printf("MOVE LF@%retval LF@returnValue\n") //IN instruction returnValue
    printf("POPFRAME\n");                      //must be always saved return value
    printf("RETURN\n\n");                      //double newline for better look after function definition
}

/**
 * Generates header of function. params is never NULL.
 */
void generate_function_header(char *fun_name, array_nodes_t params)
{
    printf("LABEL %s\n", fun_name);                                     //LABEL fun_name
    printf("PUSHFRAME\n");                                              //PUSHFRAME
    printf("DEFVAR LF@%retval");                                        //DEFVAR LF@%retval
    printf("MOVE LF@%retval nil@nil");                                  //DEFVAR LF@%retval nil@nil
    for(size_t i = 0; i < array_nodes_size(params); i++){               //DEFVAR LF@parami
        ast_t param_name = array_nodes_get(params->nodes, i);           //MOVE LF@parami LF@%i+1
        printf("DEFVAR LF@%s\n", param_name->data);                     //after this generate function body
        printf("MOVE LF@%s LF@%%d\n", param_name->data, i+1);
    }
}

/**
 * Generates code before function returns
 */
void generate_function_footer()
{
    //TODO and last value for return insert into LF@returnValue it will MOVE this value to retvalue in another function
}

/**
 * Generated code which calls function
 * @param node function call node
 * @param current_scope scope, where call is performed. Use it do deduce if it should use local or global variable
 */
void generate_function_call(scope_t current_scope, ast_t node)
{

}

/**
 * Generates code which stores return value of function to id
 */
void generate_function_call_assignment(scope_t current_scope, const char *id, ast_t func_call)
{

}

/**
 * Generates code which stores expression value to id
 */
void generate_expression_assignment(scope_t current_scope, const char *id, ast_t expression)
{


}

/**
 * Generates while label
 * WHILE$line
 */
void generate_while_label(unsigned line)
{

}

/**
 * Generate while condition
 */
void generate_while_condition(scope_t scope, queue_t expression)
{

}

/**
 * Generate WHILEELSE$line label
 */
void generate_while_else_label(unsigned line)
{

}

/**
 * Generate jump to else structure if condition was false
 */
void generate_while_jump_to_else_if_false(unsigned line)
{

}

/**
 * Jump out of while if condition is false
 */
void generate_while_jump_to_endwhile_if_false(unsigned line)
{

}

/**
 * Generate jump to WHILE$line
 */
void generate_while_jump(unsigned line)
{

}

/**
 * Generate jump to ENDWHILE$line
 */
void generate_endwhile_jump(unsigned line)
{


}

/**
 * Generates end while label
 * ENDWHILE$line
 */
void generate_endwhile(unsigned line)
{

}

#endif
