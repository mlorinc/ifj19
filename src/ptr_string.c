#include "ptr_string.h"

ptr_string_t ptr_string_new_with_length(const size_t initial_length)
{
    ptr_string_t new_ptr = (ptr_string_t) malloc(sizeof(ptr_string_t));
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

ptr_string_t ptr_string_clone(const ptr_string_t const str)
{
    ptr_string_t new_ptr = ptr_string_new_with_length(str->capacity);
    if(new_ptr != NULL)
    {
        new_ptr->length = str->length;
        for (int i = 0; i < new_ptr->length; i++)
        {
            new_ptr->buffer[i] = str->buffer[i];
        }
    }
    return new_ptr;
}

ptr_string_t ptr_string(const char *str)
{
    size_t str_len = strlen(str);
    ptr_string_tcasted_str = ptr_string_new_with_length(str_len);
    if (casted_str == NULL)
    {
        // TODO set error flag
        return NULL;
    }
    
    // Copies whole string without '\\0'
    memcpy(casted_str->buffer, str, str_len);
    casted_str->length = str_len;

}