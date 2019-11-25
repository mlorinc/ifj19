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
 * File: generator.c
 *
 */

#include "generator.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Convert hexadecimal char into decimal int.
 * <0...F> => <0...15>
 * @param c hexadecimal char
 * @return decimal int
 */
int hex_to_decimal(char c)
{
    c = toupper(c);
    if(c >= 48 && c <= 57)
    {
        return c-48;    //<0...9>
    }
    else
    {
        return c-55;    //<A...F>
    }
}


/**
 * Convert special character after escape char.
 * special characters: \" \' \\ \n \t \xhh
 * @param string where will be append special char
 * @param ifj_string string which is converting
 * @param escape_index position in ifj_string where was found escape char
 * @return number of read chars 
 */
int special_char(ptr_string_t string, ptr_string_t ifj_string, int escape_index)
{
    char after_escape = ptr_string_get_char(ifj_string, escape_index+1);
    int decimal;

    switch(after_escape)
    {
        case '\"':
        case '\'':
        case '\\':
            ptr_string_append(string, after_escape);
            return 2;
        case 'n':
            ptr_string_append(string, '\n');
            return 2;
        case 't':
            ptr_string_append(string, '\t');
            return 2;
        case 'x':
            decimal = hex_to_decimal(ptr_string_get_char(ifj_string, escape_index+3));
            decimal += (hex_to_decimal(ptr_string_get_char(ifj_string, escape_index+2))*16);
            ptr_string_append(string, decimal);
            return 4;
        default:
            ptr_string_append(string, '\\');
            return 1;
    }
}

/**
 * Convert string from language IFJ19 into normal c string.
 * @param ifj_string in corect format for language IFJ19
 * @return string without special characters
 */
ptr_string_t ifj_string_to_string(ptr_string_t ifj_string)
{
    int index_escape = 0;
    int index_start = 0;
    int index_end = ptr_string_length(ifj_string);
    ptr_string_t string = ptr_string_new();
    ptr_string_t tmp = NULL;

    while ((index_escape = ptr_string_find_char_from_index(ifj_string, '\\', index_start)) != -1)   //searching escape char
    {
        if(index_start != index_escape)
        {
            ptr_string_delete(tmp);
            tmp = ptr_string_substring(ifj_string, index_start, index_escape);
            ptr_string_concat(string, tmp);
        }
        index_start = index_escape;
        index_start += special_char(string, ifj_string, index_escape);
    }
    if(tmp == NULL)    //if ifj_string hasn't escape char
    {
        ptr_string_delete(string);
        string = ptr_string_clone(ifj_string);
    }
    else
    {
        if(index_start != index_end)    //if last char wasn't special char with escape char
        {
            tmp = ptr_string_substring(ifj_string, index_start, index_end);
            ptr_string_concat(string, tmp);
        }
        ptr_string_delete(tmp);
    }
    return string;
}

ptr_string_t ifj_string_to_code_string(ptr_string_t ifj_string)
{
    ptr_string_t string = ifj_string_to_string(ifj_string);
    ptr_string_t char_code = NULL;
    ptr_string_t tmp_char_code = NULL;
    char* string_c = ptr_string_c_string(string);
    char tmp[3];

    ptr_string_delete(string);
    string = ptr_string_new();


    for(int i=0; string_c[i] != '\0'; i++)
    {
        if(string_c[i] == '#')
        {
            char_code = ptr_string("\\035");
            ptr_string_concat(string, char_code);
            ptr_string_delete(char_code);
        }
        else if(string_c[i] == '\\')
        {
            char_code = ptr_string("\\092");
            ptr_string_concat(string, char_code);
            ptr_string_delete(char_code);
        }
        else if(string_c[i] >= 0 && string_c[i] <= 32)
        {
            sprintf(tmp, "%d", string_c[i]);
            char_code = ptr_string(tmp);
            switch (ptr_string_length(char_code))
            {
                case 1:
                    tmp_char_code = ptr_string("\\00");
                    ptr_string_insert(char_code, tmp_char_code);
                    break;
                case 2:
                    tmp_char_code = ptr_string("\\0");
                    ptr_string_insert(char_code, tmp_char_code);
                    break;
            }
            ptr_string_concat(string, char_code);
            ptr_string_delete(tmp_char_code);
            ptr_string_delete(char_code);
        }
        else
        {
            ptr_string_append(string, string_c[i]);
        }
    }
    free(string_c);
    return string;
}

ptr_string_t code_data(char* data_type, ptr_string_t value)
{
    ptr_string_t code_data = ptr_string(data_type);
    ptr_string_append(code_data, '@');
    ptr_string_concat(code_data, value);
    return code_data;
}
