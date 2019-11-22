#include "expression.h"

int STACK_SIZE = MAX_STACK;

void stackError (int error_code){
    static const char* SERR_STRINGS[MAX_SERR+1] = {"Unknown error","Stack error: INIT","Stack error: PUSH","Stack error: TOP"};
    if (error_code <= 0 || error_code > MAX_SERR)
        error_code = 0;
    printf ("%s\n", SERR_STRINGS[error_code]);
    err_flag = 1;
}

void stackInit (tStack* s) {


    if (s == NULL) {                            //kontrola ci funkcie nedostava NULL
        stackError(SERR_INIT);                  //v pripade ze podmienka == true -> raise error stackcError s kodom SERR INIT
        return;
    }
    s->top = -1;                                //iniciliaziacia "vrchu" zasobniku na -1, aby pri pushnuti bol prvy prvok na indexe 0
}

int stackEmpty (const tStack* s) {

    return s->top == -1;                        //ak je prazdny top == -1, vrati true ak nie vracia false
}

int stackFull (const tStack* s) {

    return s->top == STACK_SIZE-1;              //porovna s->top s poslednym indexom zasobnika, ak su rovne, vieme ze je zasobnik plny, vraciame true
}

void stackTop (const tStack* s, char* c) {

    if (stackEmpty(s)) {                        //ak stackEmpty == true
        stackError(SERR_TOP);                   //raise Error, error code SERR_TOP
        return;
    } else {
        *c = s->arr[s->top];                    //ak nie je prazdny mozme vratit parameter *c
    }
}


void stackPop (tStack* s) {

    if (stackEmpty(s)){                         //ak je stack prazdny
        return;
    }                                           //nedeje sa nic (nemame raisovat error)

    s->top -= 1;                                //ak nie je prazdny, znizime vrchol zasobnika
}


void stackPush (tStack* s, char c) {

    if (stackFull(s)){                          //kontrola ci je stack plny
        stackError(SERR_PUSH);                  //ak je plny nemozme pushnut dalsi prvok a raise error, code SERR_PUSH
        return;
    }
    s->top += 1;                                //ak nie je plny zvysime index kde ideme ulozit prvok na zasobnik
    s->arr[s->top] = c;                         //a priradime prvom do stacku
}
