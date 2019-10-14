#include "ptr_string.h"

ptr_string_t* ptr_string_new_with_length(const size_t initial_length)
{
    ptr_string_t* new_ptr = (ptr_string_t*) malloc(sizeof(ptr_string_t));
    if(new_ptr != NULL)
    {
        new_ptr->buffer = malloc(sizeof(char)*initial_length);
        if(new_ptr->buffer == NULL)
        {
            free(new_ptr);
            return NULL;
        }
        new_ptr->capacity = initial_length;
        new_ptr->length = 0;
    }
    return new_ptr;
}

ptr_string_t* ptr_string_clone(const ptr_string_t * const str)
{
    ptr_string_t* new_ptr = ptr_string_new_with_length(str->capacity);
    if(new_ptr != NULL)
    {
        new_ptr->length = str->length;
        memcpy(new_ptr->buffer, str->buffer, str->length);
    }
    return new_ptr;
}

char *ptr_string_c_string(ptr_string_t * const str)
{
    size_t length = str->length + 1;
    char stringValue [length];
    
    memcpy(stringValue, str->buffer, length);
    stringValue[length] = '\0';

    return stringValue;
}