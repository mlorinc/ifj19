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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifndef PTR_STRING_CHUNK
#define PTR_STRING_CHUNK (15)
#endif

struct my_string
{
    /**
     * Pointer to c string. Not ended with '\0' char
     */
    char *buffer;
    /**
     * Current length of string
     */
    size_t length;

    size_t capacity;
};

bool should_resize(ptr_string_t string, size_t extra_length)
{
    return ptr_string_length(string) + extra_length > string->capacity;
}

bool resize(ptr_string_t string, size_t extra_length)
{
    size_t new_capacity = string->capacity + extra_length + PTR_STRING_CHUNK;
    string->buffer = realloc(string->buffer, new_capacity);

    if (string->buffer == NULL)
    {
        return false;
    }

    string->capacity = new_capacity;
    return true;
}

size_t ptr_string_length(ptr_string_t string)
{
    return string->length;
}

size_t ptr_string_capacity(ptr_string_t string)
{
    return string->capacity;
}

char ptr_string_get_char(ptr_string_t string, const size_t index)
{
    assert(index < ptr_string_length(string));
    return string->buffer[index];
}

void ptr_string_set_char(ptr_string_t string, const size_t index, const char c)
{
    assert(index < ptr_string_length(string));
    string->buffer[index] = c;
}

ptr_string_t ptr_string_new()
{
    ptr_string_t str = ptr_string_new_with_length(PTR_STRING_CHUNK);
    str->length = 0;
    return str;
}

ptr_string_t ptr_string_new_with_length(const size_t initial_length)
{
    ptr_string_t new_ptr = malloc(sizeof(ptr_string_t));
    if (new_ptr != NULL)
    {
        new_ptr->buffer = malloc(sizeof(char) * initial_length);
        if (new_ptr->buffer == NULL && initial_length == 0)
        {
            free(new_ptr);
            error_exit(ERROR_INTERNAL);
        }
        new_ptr->length = initial_length;
        new_ptr->capacity = initial_length;
    }
    else
    {
        error_exit(ERROR_INTERNAL);
    }
    return new_ptr;
}

ptr_string_t ptr_string_clone(const ptr_string_t str)
{
    ptr_string_t new_ptr = ptr_string_new_with_length(str->length);
    if (new_ptr != NULL)
    {
        new_ptr->length = str->length;
        memcpy(new_ptr->buffer, str->buffer, str->length);
    }
    else
    {
        error_exit(ERROR_INTERNAL);
    }
    return new_ptr;
}

char *ptr_string_c_string(ptr_string_t const str)
{
    size_t length = str->length + 1;
    char *stringValue = (char *)malloc(sizeof(char) * length);
    if (stringValue != NULL)
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
    if (casted_str == NULL && str_len == 0)
    {
        error_exit(ERROR_INTERNAL);
        return NULL;
    }

    // Copies whole string without '\\0'
    memcpy(casted_str->buffer, str, str_len);
    return casted_str;
}

bool ptr_string_delete(ptr_string_t const str)
{
    if (str == NULL || str->buffer == NULL)
    {
        return false;
    }

    free(str->buffer);
    str->buffer = NULL;
    str->length = 0;
    str->capacity = 0;

    return true;
}

ptr_string_t ptr_string_append(ptr_string_t const str, const char c)
{
    assert(c != '\0');

    if (str == NULL || str->buffer == NULL)
        return NULL;

    if (should_resize(str, 1))
    {
        if (!resize(str, 1))
        {
            return NULL;
        }
    }

    str->buffer[ptr_string_length(str)] = c;
    str->length++;
    return str;
}

ptr_string_t ptr_string_substring(ptr_string_t const str, const size_t start, const size_t end)
{
    if (str == NULL || str->buffer == NULL)
        return NULL;
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
    if (should_resize(str, ptr_string_length(str_to_concat)))
    {
        if (!resize(str, ptr_string_length(str_to_concat)))
        {
            return NULL;
        }
    }
    memcpy(str->buffer + ptr_string_length(str), str_to_concat->buffer, str_to_concat->length);
    str->length += str_to_concat->length;
    return str;
}

ptr_string_t ptr_string_insert(ptr_string_t const str, ptr_string_t const str_to_insert)
{
    if (should_resize(str, ptr_string_length(str_to_insert)))
    {
        if (!resize(str, ptr_string_length(str_to_insert)))
        {
            return NULL;
        }
    }

    memmove(str->buffer + ptr_string_length(str_to_insert), str->buffer, ptr_string_length(str));
    memcpy(str->buffer, str_to_insert->buffer, ptr_string_length(str_to_insert));
    str->length += str_to_insert->length;
    return str;
}

size_t ptr_string_find_char(ptr_string_t const str, const char c)
{
    char *pch;
    pch = memchr(str->buffer, c, str->length);
    if (pch != NULL)
    {
        return (size_t)(pch - (str->buffer));
    }
    else
        return -1;
}

size_t ptr_string_find_char_from_index(ptr_string_t const str, const char c, const size_t index)
{
    for (size_t i = index; i < str->length; i++)
    {
        if (str->buffer[i] == c)
            return i;
    }
    return -1;
}

bool ptr_string_equals(ptr_string_t this, ptr_string_t that)
{
    assert(this != NULL && that != NULL);
    return (this == that) || (ptr_string_length(this) == ptr_string_length(that) && memcmp(this->buffer, that->buffer, ptr_string_length(this)) == 0);
}

bool ptr_string_c_equals(ptr_string_t this, char *that)
{
    assert(this != NULL && that != NULL);
    return (this->buffer == that) || (ptr_string_length(this) == strlen(that) && memcmp(this->buffer, that, ptr_string_length(this)) == 0)
}
