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
 * File: ptr_string.h
 *
 */

#ifndef __PTR_STRING_H
#define __PTR_STRING_H

#include <stddef.h>
#include <stdbool.h>

struct my_string;
typedef struct my_string *ptr_string_t;

/**
 * Get character at index
 * @param string string
 * @param index index of character from 0
 * @return character at index
 */ 
char ptr_string_get_char(ptr_string_t string, const size_t index);

/**
 * Sets character at index and MUTATES string
 * @param string string
 * @param index index where character will be placed
 */ 
void ptr_string_set_char(ptr_string_t string, const size_t index, const char c);

/**
 * Casts c string to ptr_string
 * @param str c string to be casted
 * @returns pointer to newly created string or NULL
 */ 
ptr_string_t ptr_string(const char *str);

/**
 * Creates copy of string.
 * @param str string to be copied
 * @returns pointer to newly created string or NULL
 */ 
ptr_string_t ptr_string_clone(const ptr_string_t str);

/**
 * Creates new string with default allocated capacity.
 * @returns pointer to newly created string or NULL
 */ 
ptr_string_t ptr_string_new();

/**
 * Creates new string with allocated capacity of initial_length.
 * @param initial_length initial capacity of string
 * @returns pointer to newly created string or NULL
 */ 
ptr_string_t ptr_string_new_with_length(const size_t initial_length);

/**
 * Deletes string from memory. Deleting the same string is considered as safe operation.
 * @param str string to be deleted
 * @returns bool - true if delete was successfull, false otherwise
 */ 
bool ptr_string_delete(ptr_string_t const str);

/**
 * Returns length of string.
 * @param str string
 * @returns length of string
 */ 
size_t ptr_string_length(ptr_string_t const str);

/**
 * Returns capacity of string.
 * @param str string
 * @returns capacity of string
 */ 
size_t ptr_string_capacity(ptr_string_t const str);

/**
 * Returns copy of buffer.
 * @param str string
 * @returns pointer to copy of buffer ended with null character (c string)
 */  
char *ptr_string_c_string(ptr_string_t const str);

/**
 * Cast string to unsigned integer
 * @param string string
 * @returns casted unsigned integer
 */
unsigned int ptr_string_c_string_to_int(const char *string);

/**
 * Concats two strings together. If concatenated string is larger than capacity, the string will resize itself.
 * @param str source string
 * @param str_to_concat string to be concatenated to source string
 * @returns reference to str or NULL, when error occurs
 */ 
ptr_string_t ptr_string_concat(ptr_string_t const str, ptr_string_t const str_to_concat);

/**
 * Inserts str_to_insert at beginning of str.
 * @param str source string
 * @param str_to_concat string to be inserted
 * @returns reference to str or NULL, when error occurs
 */ 
ptr_string_t ptr_string_insert(ptr_string_t const str, ptr_string_t const str_to_insert);

/**
 * Appeds character at end of str.
 * @param str source string
 * @param c character to be appended
 * @returns reference to str or NULL, when error occurs
 */ 
ptr_string_t ptr_string_append(ptr_string_t const str, const char c);

/**
 * Returns substring from start to end.
 * @param str source string
 * @param start start index (inclusive)
 * @param end end index (exclusive)
 * @returns reference to str or NULL, when error occurs
 */ 
ptr_string_t ptr_string_substring(ptr_string_t const str, const size_t start, const size_t end);

/**
 * Finds character in string and returns its index.
 * @param str source string
 * @param c character to be looked for
 * @returns index of character or -1 if character was not found
 */ 
size_t ptr_string_find_char(ptr_string_t const str, const char c);

/**
 * Finds character in string and returns its index. Searching is started from index.
 * @param str source string
 * @param c character to be looked for
 * @param index starting position of search
 * @returns index of character or -1 if character was not found
 */ 
size_t ptr_string_find_char_from_index(ptr_string_t const str, const char c, const size_t index);

/**

 * Compares two strings
 * @param this string a
 * @param that string b
 * @return true if strings are equal
 */ 
bool ptr_string_equals(ptr_string_t this, ptr_string_t that);

/**
 * Compares two strings
 * @param this string a
 * @param that string b
 * @return true if strings are equal
 */ 
bool ptr_string_c_equals(ptr_string_t this, char *that);

 * Delete last char in string.
 * @param str source string
 */
void ptr_string_delete_last(ptr_string_t str);

static inline size_t should_resize(size_t capacity, size_t length1, size_t length2) {
    return capacity < length1 + length2;
}

#endif
