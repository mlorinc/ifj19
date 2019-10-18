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
#include <assert.h>

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

ptr_string_t ptr_string_append(ptr_string_t const str, const char c) {
    assert(c != '\0');

    if(str == NULL || str->buffer == NULL) return NULL;

    ptr_string_t newStr = ptr_string_new_with_length(str->length + 1);

    if (newStr == NULL)
    {
        return NULL;
    }
    
    memcpy(newStr->buffer, str->buffer, str->length);
    newStr->buffer[str->length] = c;

    return newStr;
}

ptr_string_t ptr_string_substring(ptr_string_t const str, const size_t start, const size_t end) {
    if(str == NULL || str->buffer == NULL) return NULL;
    assert(end > start);
    assert(str->length > start && str->length <= end);

    size_t length = end - start;
    ptr_string_t newStr = ptr_string_new_with_length(length);

    if (newStr == NULL)
    {
        return NULL;
    }

    memcpy(newStr->buffer, str->buffer + start, length);

    return newStr;
}

ptr_string_t ptr_string_concat(ptr_string_t const str, ptr_string_t const str_to_concat)
{
    ptr_string_t newStr = ptr_string_new_with_length(str->length + str_to_concat->length);
    memcpy(newStr->buffer, str->buffer, str->length);
    strcat(newStr->buffer, str_to_concat->buffer);

    if (newStr == NULL)
    {
        return NULL;
    }

    return newStr;
}

ptr_string_t ptr_string_insert(ptr_string_t const str, ptr_string_t const str_to_insert)
{
    ptr_string_t newStr = ptr_string_new_with_length(str->length + str_to_insert->length);
    memcpy(newStr->buffer, str_to_insert->buffer, str_to_insert->length);
    strcat(newStr->buffer, str->buffer);

    if (newStr == NULL)
    {
        return NULL;
    }

    return newStr;
}

size_t ptr_string_find_char(ptr_string_t const str, const char c)
{
    char * pch;
    pch = memchr(str->buffer, c, str->length);
    if (pch != NULL)
    {
        return (size_t)(pch-(str->buffer));
    }
    else
        return -1;
}

size_t ptr_string_find_char_from_index(ptr_string_t const str, const char c, const size_t index)
{
    //TODO ZMAZAT TUTO KOKOTINU
    //TATO KOKOTINA POD TYMTO KOMENTAROM ABSOLUTNE NEFUNGUJE, CHCEL SOM TO SPRAVIT NEJAK EFEKTIVNE
    //ALE MAM TO PICI TAKTO, LEBO CEZ WHILE TO BUDE ROBIT 45616516156 ROKOV A TOTO PROSTE NEFUNGUJE
    //LEBO SI KOKOT HLADA ADRESY KDE NEMA

    /*char * pch;
    pch = memchr(str->buffer, c, str->length);
    if (pch != NULL)
    {
        size_t actIndex = (size_t)(pch-(str->buffer));
        if (actIndex > str->length){                    //ak bude novy "index" za medzami dlzky stringu  vratime -1 -> nenasli sme
            return -1;
        }
        else if (actIndex < index){                     //ak je novy index mensi ako hladany musime pokracovat v hladani
            ptr_string_find_char_from_index(str, c, index);
        }
        else if (actIndex >= index){
            printf("%d\n", actIndex);
            return actIndex;
        }
    }*/
}