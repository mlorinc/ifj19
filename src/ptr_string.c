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
        new_ptr->length = initial_length;
    }
    return new_ptr;
}

ptr_string_t* ptr_string_clone(const ptr_string_t * const str)
{
    ptr_string_t* new_ptr = ptr_string_new_with_length(str->length);
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
    char* stringValue = (char*) malloc(sizeof(char)*length);
    if(stringValue != NULL)
    {
        memcpy(stringValue, str->buffer, length - 1);
        stringValue[length] = '\0';
    }

    return stringValue;
}

ptr_string_t* ptr_string(const char *str)
{
    size_t str_len = strlen(str);
    ptr_string_t *casted_str = ptr_string_new_with_length(str_len);
    
    // Copies whole string without '\\0'
    memcpy(casted_str->buffer, str, str_len);
    casted_str->length = str_len;

}