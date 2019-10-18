#include "ptr_string.h"

int main (void){
    ptr_string_t str = ptr_string_new_with_length(7);
    str->buffer = "Ahdjo";
    printf("%ld\n",ptr_string_find_char_from_index(str, 'o', 0));
}