#include "queue.h"
#include "generator_functions.h"
#include "deque.c"
#include "scope.c"
#include "scaner.h"
#include "string_convertor.c"
#include <stdbool.h>

void generate_expression(scope_t scope, queue_t expression)
{
    while (!queue_empty(expression)){
        tToken *token = queue_pop(expression);
        if (is_literal(token)){
            char *buffer = ptr_string_c_string(token->value);
            if (token->type == TFLOAT){
                printf("PUSHS float@%a\n", atof(buffer));
            }
            else {
                printf("PUSHS %s@%s\n", token->type == TINT ? "int" : "string",buffer);
            }
            free(buffer);
        }

        else if (token->type == TIDENTIFICATOR) {
            char *buffer = ptr_string_c_string(token->value);
            scope_t variable_scope = find_scope_with_defined_variable(scope, token->value);
            if (variable_scope->root->node_type == FUNCTION_DEFINITION) {
                printf("PUSHS LF@%s\n", buffer);
            }
            else {
                printf("PUSHS GF@%s\n", buffer);
            }
            free(buffer);
        }
        else {
            switch(token->type){
                case TADD:
                    printf("ADDS\n");
                    break;
                case TSUB:
                    printf("SUBS\n");
                    break;
                case TMUL:
                    printf("MULS\n");
                    break;
                case TDIV:
                    printf("SUBS\n");
                    break;
                default:
                    fprintf(stderr, "Zabudol som sorry :(\n");
                    break;
            }
        }
    }
}

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
            printf("DEFVAR GF@cond%u\n", line);
            printf("GT GF@cond%u GF@%s GF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
            printf("JUMPIFEQ %s%u bool@true GF@cond%u\n", label_name, line, line);
        }

        else if (!strcmp(expression->last, '<')) {
            printf("DEFVAR GF@cond%u\n", line);
            printf("LT GF@cond%u GF@%s GF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
            printf("JUMPIFEQ %s%u bool@true GF@cond%u\n", label_name, line, line);
        }

        else if (!strcmp(expression->last, "==")) {
            printf("JUMPIFEQ %s%u GF@%s GF@%s\n", label_name, line, (char *) expression->first->value, (char *) expression->first->next->value);
        }

        else if(!strcmp(expression->last, ">=")){
            printf("DEFVAR GF@condgt%u\n", line);
            printf("DEFVAR GF@condeq%u\n", line);
            printf("GT GF@condgt%u GF@%s GF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
            printf("EQ GF@condeq%u GF@%s GF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
            printf("DEFVAR GF@iseq%u\n", line);
            printf("OR GF@iseq%u GF@condeq%u GF@condgt%u\n", line, line, line);
            printf("JUMPIFEQ %s%u bool@true GF@iseq%u\n", label_name, line, line);
        }

        else if(!strcmp(expression->last, "<=")){
            printf("DEFVAR GF@condlt%u\n", line);
            printf("DEFVAR GF@condeq%u\n", line);
            printf("LT GF@condlt%u GF@%s GF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
            printf("EQ GF@condeq%u GF@%s GF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
            printf("DEFVAR GF@iseq%u\n", line);
            printf("OR GF@iseq%u GF@condeq%u GF@condlt%u\n", line, line, line);
            printf("JUMPIFEQ %s%u bool@true GF@iseq%u\n", label_name, line, line);
        }
    }

    else { //we are in LOCAL FRAME

        if (!strcmp(expression->last, '>')) {
            printf("DEFVAR LF@cond%u\n", line);
            printf("GT LF@cond%u LF@%s LF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
            printf("JUMPIFEQ %s%u bool@true LF@cond%u\n", label_name, line, line);
        }

        else if (!strcmp(expression->last, '<')) {
            printf("DEFVAR LF@cond%u\n", line);
            printf("LT LF@cond%u LF@%s LF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
            printf("JUMPIFEQ %s%u bool@true LF@cond%u\n", label_name, line, line);
        }

        else if (!strcmp(expression->last, "==")) {
            printf("JUMPIFEQ %s%u LF@%s LF@%s\n", label_name, line, (char *) expression->first->value, (char *) expression->first->next->value);
        }

        else if(!strcmp(expression->last, ">=")){
            printf("DEFVAR LF@condgt%u\n", line);
            printf("DEFVAR LF@condeq%u\n", line);
            printf("GT LF@condgt%u LF@%s LF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
            printf("EQ LF@condeq%u LF@%s LF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
            printf("DEFVAR LF@iseq%u\n", line);
            printf("OR LF@iseq%u LF@condeq%u LF@condgt%u\n", line, line, line);
            printf("JUMPIFEQ %s%u bool@true LF@iseq%u\n", label_name, line, line);
        }

        else if(!strcmp(expression->last, "<=")){
            printf("DEFVAR LF@condlt%u\n", line);
            printf("DEFVAR LF@condeq%u\n", line);
            printf("LT LF@condlt%u LF@%s LF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
            printf("EQ LF@condeq%u LF@%s LF@%s\n", line, (char *) expression->first->value, (char *) expression->first->next->value);
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



bool is_literal(tToken *token)
{
    return token->type == TINT || token->type == TFLOAT ||  token->type == TSTRING;
}

/**
 * Generates return. If expression is NULL, it means it returns None
 */
void generate_return(scope_t scope, queue_t expression)
{
    if (expression == NULL){
        printf("MOVE LF@%retval nil@nil\n");
        printf("POPFRAME\n");
        printf("RETURN\n\n");
        return;
    }

    generate_expression(scope, expression);

    printf("POPS LF@%retval");
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
    //when we dont want to return anything from function
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
    printf("CREATEFRAME\n");

    for(size_t i = 0; i < array_nodes_size(params); i++){               //DEFVAR TF@%1
        ast_t param_name = array_nodes_get(params->nodes, i);           //MOVE LF@parami LF@%i+1
        printf("DEFVAR LF@%s\n", param_name->data);                     //after this generate function body
        printf("MOVE LF@%s LF@%%d\n", param_name->data, i+1);
    }
}

/**
 * Generates code which stores return value of function to id
 */
void generate_function_call_assignment(scope_t current_scope, const char *id, ast_t func_call)
{
    for(size_t i = 0; i < array_nodes_size(func_call->nodes); i++){
        ast_t param = array_nodes_get(func_call->nodes, i);
        char *buffer = ptr_string_c_string(param->data);

        printf("DEFVAR TF@%%u\n", i+1);

        char *temp;
        switch(param->node_type){
            case STRING_LITERAL:
                temp = ptr_string_c_string(ifj_string_to_code_string(param->data));
                printf("MOVE TF@%%u string@%%s\n", i+1, temp);
                free(temp);
                break;
            case FLOAT_LITERAL:
                printf("MOVE TF@%%u float@%%a\n", i+1, atof(buffer));
                break;
            case INT_LITERAL:
                printf("MOVE TF@%%u int@%%s\n", i+1, buffer);
                break;
            case ID:
                char *buffer = ptr_string_c_string(param->data);
                if (is_local_frame(current_scope, param->data)) {
                    printf("MOVE TF@%%u LF@%%s\n", i+1, buffer);
                }
                else {
                    printf("MOVE TF@%%u GF@%%s\n", i+1, buffer);
                }
                break;
        }

        free(buffer);
    }

    printf("CALL %s\n", func_call->data);
    if (is_local_frame(current_scope, ptr_string(id))){
        printf("MOVE LF@%s TF@%retval\n", id);
    }
    else{
        printf("MOVE GF@%s TF@%retval\n", id);
    }
}

bool is_local_frame(scope_t scope, ptr_string_t id)
{
    scope_t return_scope = find_scope_with_defined_variable(scope, id);
    return return_scope->root->node_type == FUNCTION_DEFINITION ? true : false;
}

/**
 * Generates code which stores expression value to id
 */
void generate_expression_assignment(scope_t current_scope, const char *id, ast_t expression)
{
    generate_expression(current_scope)

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
