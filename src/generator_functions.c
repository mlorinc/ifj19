#include "queue.h"
#include "generator_functions.h"
#include "scaner.h"
#include "string_convertor.h"
#include "array_nodes.h"
#include <stdbool.h>

bool is_literal(tToken *token)
{
    return token->type == TINT || token->type == TFLOAT ||  token->type == TSTRING;
}

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
                case TGT:
                    printf("GTS\n");
                    break;
                case TGTE:
                case TLT:
                    printf("LTS\n");
                    break;
                case TLTE:
                case TEQ:
                    printf("EQS\n");
                    break;
                case TNE:
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
void generate_condition(scope_t scope, queue_t expression, char *label_name, size_t line)
{
    if(expression == NULL){
        printf("JUMP %s$%zu\n", label_name, line);
    }
    else{
        generate_expression(scope, expression);
        printf("PUSHS bool@true\n");
        printf("JUMPIFEQS %s$%zu\n", label_name, line);
    }

    /*if (!strcmp(expression->last, '>')) {
        printf("DEFVAR GF@cond%zu\n", line);
        printf("GT GF@cond%zu GF@%s GF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("JUMPIFEQ %s%zu bool@true GF@cond%zu\n", label_name, line, line);
    } else if (!strcmp(expression->last, '<')) {
        printf("DEFVAR GF@cond%zu\n", line);
        printf("LT GF@cond%zu GF@%s GF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("JUMPIFEQ %s%zu bool@true GF@cond%zu\n", label_name, line, line);
    } else if (!strcmp(expression->last, "==")) {
        printf("JUMPIFEQ %s%zu GF@%s GF@%s\n", label_name, line, (char *) expression->first->value,
               (char *) expression->first->next->value);
    } else if (!strcmp(expression->last, ">=")) {
        printf("DEFVAR GF@condgt%zu\n", line);
        printf("DEFVAR GF@condeq%zu\n", line);
        printf("GT GF@condgt%zu GF@%s GF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("EQ GF@condeq%zu GF@%s GF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("DEFVAR GF@iseq%zu\n", line);
        printf("OR GF@iseq%zu GF@condeq%zu GF@condgt%zu\n", line, line, line);
        printf("JUMPIFEQ %s%zu bool@true GF@iseq%zu\n", label_name, line, line);
    } else if (!strcmp(expression->last, "<=")) {
        printf("DEFVAR GF@condlt%zu\n", line);
        printf("DEFVAR GF@condeq%zu\n", line);
        printf("LT GF@condlt%zu GF@%s GF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("EQ GF@condeq%zu GF@%s GF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("DEFVAR GF@iseq%zu\n", line);
        printf("OR GF@iseq%zu GF@condeq%zu GF@condlt%zu\n", line, line, line);
        printf("JUMPIFEQ %s%zu bool@true GF@iseq%zu\n", label_name, line, line);
    }

    if (!strcmp(expression->last, '>')) {
        printf("DEFVAR LF@cond%zu\n", line);
        printf("GT LF@cond%zu LF@%s LF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("JUMPIFEQ %s%zu bool@true LF@cond%zu\n", label_name, line, line);
    } else if (!strcmp(expression->last, '<')) {
        printf("DEFVAR LF@cond%zu\n", line);
        printf("LT LF@cond%zu LF@%s LF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("JUMPIFEQ %s%zu bool@true LF@cond%zu\n", label_name, line, line);
    } else if (!strcmp(expression->last, "==")) {
        printf("JUMPIFEQ %s%zu LF@%s LF@%s\n", label_name, line, (char *) expression->first->value,
               (char *) expression->first->next->value);
    } else if (!strcmp(expression->last, ">=")) {
        printf("DEFVAR LF@condgt%zu\n", line);
        printf("DEFVAR LF@condeq%zu\n", line);
        printf("GT LF@condgt%zu LF@%s LF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("EQ LF@condeq%zu LF@%s LF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("DEFVAR LF@iseq%zu\n", line);
        printf("OR LF@iseq%zu LF@condeq%zu LF@condgt%zu\n", line, line, line);
        printf("JUMPIFEQ %s%zu bool@true LF@iseq%zu\n", label_name, line, line);
    } else if (!strcmp(expression->last, "<=")) {
        printf("DEFVAR LF@condlt%zu\n", line);
        printf("DEFVAR LF@condeq%zu\n", line);
        printf("LT LF@condlt%zu LF@%s LF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("EQ LF@condeq%zu LF@%s LF@%s\n", line, (char *) expression->first->value,
               (char *) expression->first->next->value);
        printf("DEFVAR LF@iseq%zu\n", line);
        printf("OR LF@iseq%zu LF@condeq%zu LF@condlt%zu\n", line, line, line);
        printf("JUMPIFEQ %s%zu bool@true LF@iseq%zu\n", label_name, line, line);
    }*/
}

/**
 * Generates ENDIF label for if elif else sequence
 * LABEL ENDIF$line
 * @param line
 */
void generate_endif(size_t line)
{
    printf("LABEL ENDIF$%zu\n", line);
}

/**
 * Generates ENDIF jump for if elif else. It is used as last command to get out of if conditions.
 * jmp ENDIF$line
 * @param line
 */
void generate_endif_jump(size_t line)
{
    printf("JUMP ENDIF$%zu\n", line);
}

/**
 * Generates return. If expression is NULL, it means it returns None
 */
void generate_return(scope_t scope, queue_t expression)
{
    if (expression == NULL){
        printf("MOVE LF@%%retval nil@nil\n");
        printf("POPFRAME\n");
        printf("RETURN\n\n");
        return;
    }

    generate_expression(scope, expression);

    printf("POPS LF@%%retval");
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
    printf("DEFVAR LF@%%retval");                                        //DEFVAR LF@%retval
    printf("MOVE LF@%%retval nil@nil");                                  //DEFVAR LF@%retval nil@nil
    for(size_t i = 0; i < array_nodes_size(params); i++){               //DEFVAR LF@parami
        ast_t param_name = array_nodes_get(params, i);           //MOVE LF@parami LF@%i+1
        char *buffer = ptr_string_c_string(param_name->data);
        printf("DEFVAR LF@%s\n", buffer);                               //after this generate function body
        printf("MOVE LF@%s LF@%%%zu\n", buffer, i+1);
        free(buffer);
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

bool is_local_frame(scope_t scope, ptr_string_t id)
{
    scope_t return_scope = find_scope_with_defined_variable(scope, id);
    return return_scope != NULL;
}

/**
 * Generated code which calls function
 * @param node function call node
 * @param current_scope scope, where call is performed. Use it do deduce if it should use local or global variable
 */
void generate_function_call(scope_t current_scope, ast_t func_call)
{
    printf("CREATEFRAME\n");

    for(size_t i = 0; i < array_nodes_size(func_call->nodes); i++){
        ast_t param = array_nodes_get(func_call->nodes, i);
        char *buffer = ptr_string_c_string(param->data);

        printf("DEFVAR TF@%%%zu\n", i+1);

        char *temp;
        switch(param->node_type){
            case STRING_LITERAL:
                temp = ptr_string_c_string(ifj_string_to_code_string(param->data));
                printf("MOVE TF@%%%zu string@%%%s\n", i+1, temp);
                free(temp);
                break;
            case FLOAT_LITERAL:
                printf("MOVE TF@%%%zu float@%%%a\n", i+1, atof(buffer));
                break;
            case INT_LITERAL:
                printf("MOVE TF@%%%zu int@%%%s\n", i+1, buffer);
                break;
            case ID:
                temp = ptr_string_c_string(param->data);
                if (is_local_frame(current_scope, param->data)) {
                    printf("MOVE TF@%%%zu LF@%%%s\n", i+1, temp);
                }
                else {
                    printf("MOVE TF@%%%zu GF@%%%s\n", i+1, temp);
                }
                free(temp);
                break;
            default:
                fprintf(stderr, "FAJRONT PICO\n");
        }

        free(buffer);
    }

    printf("CALL %s\n", ptr_string_c_string(func_call->data));
}

/**
 * Generates code which stores return value of function to id
 */
void generate_function_call_assignment(scope_t current_scope, const char *id, ast_t func_call)
{
    for(size_t i = 0; i < array_nodes_size(func_call->nodes); i++){
        ast_t param = array_nodes_get(func_call->nodes, i);
        char *buffer = ptr_string_c_string(param->data);

        printf("DEFVAR TF@%%%zu\n", i+1);

        char *temp;
        switch(param->node_type){
            case STRING_LITERAL:
                temp = ptr_string_c_string(ifj_string_to_code_string(param->data));
                printf("MOVE TF@%%%zu string@%%%s\n", i+1, temp);
                free(temp);
                break;
            case FLOAT_LITERAL:
                printf("MOVE TF@%%%zu float@%%%a\n", i+1, atof(buffer));
                break;
            case INT_LITERAL:
                printf("MOVE TF@%%%zu int@%%%s\n", i+1, buffer);
                break;
            case ID:
                temp = ptr_string_c_string(param->data);
                if (is_local_frame(current_scope, param->data)) {
                    printf("MOVE TF@%%%zu LF@%%%s\n", i+1, temp);
                }
                else {
                    printf("MOVE TF@%%%zu GF@%%%s\n", i+1, temp);
                }
                free(temp);
                break;
            default:
                fprintf(stderr, "FAJRONT PICO\n");
        }

        free(buffer);
    }

    printf("CALL %s\n", ptr_string_c_string(func_call->data));
    if (is_local_frame(current_scope, ptr_string(id))){
        printf("POPS LF@%s\n", id);
    }
    else{
        printf("POPS GF@%s\n", id);
    }
}

/**
 * Generates code which stores expression value to id
 */
void generate_expression_assignment(scope_t current_scope, const char *id, ast_t expression)
{
    generate_expression(current_scope, expression->data);

    if (is_local_frame(current_scope, ptr_string(id))){
        printf("POPS LF@%s\n", id);
    }
    else{
        printf("POPS GF@%s\n", id);
    }
}

/**
 * Generates while label
 * WHILE$line
 */
void generate_while_label(size_t line)
{
    printf("LABEL WHILE$%zu\n", line);
}

/**
 * Generate while condition
 */
void generate_while_condition(scope_t scope, queue_t expression)
{
    generate_expression(scope,expression);
}

/**
 * Generate WHILEELSE$line label
 */
void generate_while_else_label(size_t line)
{
    printf("LABEL WHILEELSE$%zu\n", line);
}

/**
 * Generate jump to else structure if condition was false
 */
void generate_while_jump_to_else_if_false(size_t line)
{
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS WHILEELSE$%zu\n", line);
}

/**
 * Jump out of while if condition is false
 */
void generate_while_jump_to_endwhile_if_false(size_t line)
{
    printf("PUSHS bool@false\n");
    printf("JUMPIFEQS ENDWHILE$%zu\n", line);
}

/**
 * Generate jump to WHILE$line
 */
void generate_while_jump(size_t line)
{
    printf("JUMP WHILE$%zu\n", line);
}

/**
 * Generate jump to ENDWHILE$line
 */
void generate_endwhile_jump(size_t line)
{
    printf("JUMP ENDWHILE$%zu\n", line);
}

/**
 * Generates end while label
 * ENDWHILE$line
 */
void generate_endwhile(size_t line)
{
    printf("LABEL ENDWHILE$%zu\n",line);
}
