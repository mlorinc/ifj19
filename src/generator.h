/**
 * IFJ2019 @ VUT FIT Brno
 * 25.11.2019
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
 * File: generator.h
 *
 */

#ifndef IFJ19_GENERATOR_H
#define IFJ19_GENERATOR_H

#include "ptr_string.h"

/**
 * Create data type in format 'data type'@'value'.
 * @param data_type type of data
 * @param value value
 * @return type'@'value'
 */
ptr_string_t code_data(char* data_type, ptr_string_t value);

/**
 * Convert string from IFJ19 to IFJcode19.
 * @param ifj_string in corect format for language IFJ19
 * @return string in corect format for language IFJcode19
 */
ptr_string_t ifj_string_to_code_string(ptr_string_t ifj_string);

#endif