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
            printf("DEFVAR LF@cond%u\n", line);
            printf("GT GF@cond%u GF@%s GF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("JUMPIFEQ %s%u bool@true GF@cond%u\n", label_name, line, line);
        }

        else if (!strcmp(expression->last, '<')) {
            printf("DEFVAR LF@cond%u\n", line);
            printf("LT GF@cond%u GF@%s GF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("JUMPIFEQ %s%u bool@true GF@cond%u\n", label_name, line, line);
        }

        else if (!strcmp(expression->last, "==")) {
            printf("JUMPIFEQ %s%u GF@%s GF@%s\n", label_name, line, (char *) expression->first, (char *) expression->first->next);
        }

        else if(!strcmp(expression->last, ">=")){
            printf("DEFVAR GF@condgt%u\n", line);
            printf("DEFVAR GF@condeq%u\n", line);
            printf("GT GF@condgt%u GF@%s GF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("EQ GF@condeq%u GF@%s GF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("DEFVAR GF@iseq%u\n", line);
            printf("OR GF@iseq%u GF@condeq%u GF@condgt%u\n", line, line, line);
            printf("JUMPIFEQ %s%u bool@true GF@iseq%u\n", label_name, line, line);
        }

        else if(!strcmp(expression->last, "<=")){
            printf("DEFVAR GF@condlt%u\n", line);
            printf("DEFVAR GF@condeq%u\n", line);
            printf("LT GF@condlt%u GF@%s GF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("EQ GF@condeq%u GF@%s GF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("DEFVAR GF@iseq%u\n", line);
            printf("OR GF@iseq%u GF@condeq%u GF@condlt%u\n", line, line, line);
            printf("JUMPIFEQ %s%u bool@true GF@iseq%u\n", label_name, line, line);
        }
    }

    else { //we are in LOCAL FRAME

        if (!strcmp(expression->last, '>')) {
            printf("DEFVAR LF@cond%u\n", line);
            printf("GT LF@cond%u LF@%s LF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("JUMPIFEQ %s%u bool@true LF@cond%u\n", label_name, line, line);
        }

        else if (!strcmp(expression->last, '<')) {
            printf("LT LF@cond%u LF@%s LF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("JUMPIFEQ %s%u bool@true LF@cond%u\n", label_name, line, line);
        }

        else if (!strcmp(expression->last, "==")) {
            printf("JUMPIFEQ %s%u LF@%s LF@%s\n", label_name, line, (char *) expression->first, (char *) expression->first->next);
        }

        else if(!strcmp(expression->last, ">=")){
            printf("DEFVAR LF@condgt%u\n", line);
            printf("DEFVAR LF@condeq%u\n", line);
            printf("GT LF@condgt%u LF@%s LF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("EQ LF@condeq%u LF@%s LF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("DEFVAR LF@iseq%u\n", line);
            printf("OR LF@iseq%u LF@condeq%u LF@condgt%u\n", line, line, line);
            printf("JUMPIFEQ %s%u bool@true LF@iseq%u\n", label_name, line, line);
        }

        else if(!strcmp(expression->last, "<=")){
            printf("DEFVAR LF@condlt%u\n", line);
            printf("DEFVAR LF@condeq%u\n", line);
            printf("LT LF@condlt%u LF@%s LF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("EQ LF@condeq%u LF@%s LF@%s\n", line, (char *) expression->first, (char *) expression->first->next);
            printf("DEFVAR LF@iseq%u\n", line);
            printf("OR LF@iseq%u LF@condeq%u LF@condlt%u\n", line, line, line);
            printf("JUMPIFEQ %s%u bool@true LF@iseq%u\n", label_name, line, line);
        }
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
    printf("POPFRAME\n");
    printf("RETURN\n");
    //we dont have to move any return value into @%retval despite of fact, that we declarate it... of course in case,
    //when we dont want to return anything for function
    //f.e.: def foo(a,b):
    //          print(a,b)
    //          return
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
