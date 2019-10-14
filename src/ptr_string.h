#ifndef __PTR_STRING_H
#define __PTR_STRING_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct ptr_string {
    /**
     * Pointer to c string. Not ended with '\0' char
     */
    char *buffer;
    /**
     * Current length of string
     */ 
    size_t length;
} ptr_string_t;

/**
 * Creates new string with size of __PTR_STRING_CHUNK
 * @returns pointer to newly created string or NULL
 */
ptr_string_t* ptr_string_new();

/**
 * Casts c string to ptr_string
 * @param str c string to be casted
 * @returns pointer to newly created string or NULL
 */ 
ptr_string_t* ptr_string(const char *str);

/**
 * Creates copy of string.
 * @param str string to be copied
 * @returns pointer to newly created string or NULL
 */ 
ptr_string_t* ptr_string_clone(const ptr_string_t * const str);

/**
 * Creates new string with allocated capacity of initial_length.
 * @param initial_length initial capacity of string
 * @returns pointer to newly created string or NULL
 */ 
ptr_string_t* ptr_string_new_with_length(const size_t initial_length);

/**
 * Deletes string from memory. Deleting the same string is considered as safe operation.
 * @param str string to be deleted
 * @returns bool - true if delete was successfull, false otherwise
 */ 
bool ptr_string_delete(ptr_string_t * const str);

/**
 * Returns length of string.
 * @param str string
 * @returns length of string
 */ 
size_t ptr_string_length(ptr_string_t * const str);

/**
 * Returns capacity of string.
 * @param str string
 * @returns capacity of string
 */ 
size_t ptr_string_capacity(ptr_string_t * const str);

/**
 * Returns copy of buffer.
 * @param str string
 * @returns pointer to copy of buffer ended with null character
 */  
char *ptr_string_c_string(ptr_string_t * const str);

/**
 * Concats two strings together. If concanetated string is larger than capacity, the string will resize itself.
 * @param str source string
 * @param str_to_concat string to be concanetated to source string
 * @returns new concanetated string or NULL, when error occurs
 */ 
ptr_string_t* ptr_string_concat(ptr_string_t *const str, ptr_string_t *const str_to_concat);

/**
 * Inserts str_to_insert at begging of str.
 * @param str source string
 * @param str_to_concat string to be inserted
 * @returns new string or NULL, when error occurs
 */ 
ptr_string_t* ptr_string_insert(ptr_string_t *const str, ptr_string_t *const str_to_insert);

/**
 * Appeds character at end of str.
 * @param str source string
 * @param c character to be appended
 * @returns new string or NULL, when error occurs
 */ 
ptr_string_t* ptr_string_append(ptr_string_t *const str, const char c);

/**
 * Returns substring from start to end.
 * @param str source string
 * @param start start index (inclusive)
 * @param end end index (exclusive)
 * @returns new string or NULL, when error occurs
 */ 
ptr_string_t* ptr_string_substring(ptr_string_t *const str, const size_t start, const size_t end);

/**
 * Finds character in string and returns its index.
 * @param str source string
 * @param c character to be looked for
 * @returns index of character or -1 if character was not found
 */ 
size_t ptr_string_find_char(ptr_string_t *const str, const char c);

/**
 * Finds character in string and returns its index. Searching is started from index.
 * @param str source string
 * @param c character to be looked for
 * @param index starting position of search
 * @returns index of character or -1 if character was not found
 */ 
size_t ptr_string_find_char_from_index(ptr_string_t *const str, const char c, const size_t index);

static inline size_t should_resize(size_t capacity, size_t length1, size_t length2) {
    return capacity < length1 + length2;
}

#endif
