/**
 * IFJ2019 @ VUT FIT Brno
 * 13.10.2019
 *
 * Variant 2
 * Team: 054
 *
 * Authors:
 * Marian Lorinc (xlorin01)
 * Lukas Javorsky (xjavor20)
 * Patrik Ondriga (xondri08)
 * Peter Vinarcik (xvinar00)
 *
 * File: ptr_string.c
 *
 */

#include "ptr_string.h"
#include "error.h"

ptr_string_t* ptr_string_new_with_length(const size_t initial_length)
{
    ptr_string_t* new_ptr = (ptr_string_t*) malloc(sizeof(ptr_string_t));
    if(new_ptr != NULL)
    {
        new_ptr->buffer = malloc(sizeof(char)*initial_length);
        if(new_ptr->buffer == NULL)
        {
            free(new_ptr);
            error_exit(ERROR_INTERNAL);
        }
        new_ptr->capacity = initial_length;
        new_ptr->length = 0;
    }
    else {
        error_exit(ERROR_INTERNAL);
    }
    return new_ptr;
}

ptr_string_t* ptr_string_clone(const ptr_string_t * const str)
{
    ptr_string_t* new_ptr = ptr_string_new_with_length(str->capacity);
    if(new_ptr != NULL)
    {
        new_ptr->length = str->length;
        for (int i = 0; i < new_ptr->length; i++)
        {
            new_ptr->buffer[i] = str->buffer[i];
        }
    }
    else{
        error_exit(ERROR_INTERNAL);
    }
    return new_ptr;
}

ptr_string_t* ptr_string(const char *str)
{
    size_t str_len = strlen(str);
    ptr_string_t *casted_str = ptr_string_new_with_length(str_len);

    // Copies whole string without '\\0'
    memcpy(casted_str->buffer, str, str_len);
    casted_str->length = str_len;

}

ptr_string_t* ptr_string_new()
{
    ptr_string_t* new_ptr = ptr_string_new_with_length(__PTR_STRING_CHUNK);

    return new_ptr;
}
