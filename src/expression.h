#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>

#define MAX_STACK 20
extern int STACK_SIZE;

extern int err_flag;

#define MAX_SERR    3
#define SERR_INIT   1
#define SERR_PUSH   2
#define SERR_TOP    3

typedef struct {
    char arr[MAX_STACK];
    int top;
} tStack;


void stackError ( int error_code );
void stackInit ( tStack* s );
int stackEmpty ( const tStack* s );
int stackFull ( const tStack* s );
void stackTop ( const tStack* s, char* c );
void stackPop ( tStack* s );
void stackPush ( tStack* s, char c );

#endif

