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

ptr_string_t ptr_string_new_with_length(const size_t initial_length)
{
    ptr_string_t new_ptr = (ptr_string_t) malloc(sizeof(ptr_string_t));
    if(new_ptr != NULL)
    {
        new_ptr->buffer = malloc(sizeof(char)*initial_length);
        if(new_ptr->buffer == NULL)
        {
            free(new_ptr);
            error_exit(ERROR_INTERNAL);
        }
        new_ptr->length = initial_length;
    }
    else{
        error_exit(ERROR_INTERNAL);
    }
    return new_ptr;
}

ptr_string_t ptr_string_clone(const ptr_string_t const str)
{
    ptr_string_t new_ptr = ptr_string_new_with_length(str->length);
    if(new_ptr != NULL)
    {
        new_ptr->length = str->length;
        memcpy(new_ptr->buffer, str->buffer, str->length);
    }
    else{
        error_exit(ERROR_INTERNAL);
    }
    return new_ptr;
}

char *ptr_string_c_string(ptr_string_t const str)
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

ptr_string_t ptr_string(const char *str)
{
    size_t str_len = strlen(str);
    ptr_string_t casted_str = ptr_string_new_with_length(str_len);
    if (casted_str == NULL)
    {
        // TODO set error flag
        return NULL;
    }

    // Copies whole string without '\\0'
    memcpy(casted_str->buffer, str, str_len);
    casted_str->length = str_len;
    return casted_str;
}

ptr_string_t* ptr_string_new()
{
    ptr_string_t* new_ptr = ptr_string_new_with_length(__PTR_STRING_CHUNK);

    return new_ptr;
}

/**
 * Deletes string from memory. Deleting the same string is considered as safe operation.
 * @param str string to be deleted
 * @author Marian Lorinc
 * @returns bool - true if delete was successfull, false otherwise
 */ 
bool ptr_string_delete(ptr_string_t const str) {
    if (str == NULL || str->buffer == NULL) {
        return false;
    }

    free(str->buffer);
    str->buffer = NULL;
    str->length = 0;
    
    return true;
}
