#ifndef L2AUTH_CORE_L2_STRING_H
#define L2AUTH_CORE_L2_STRING_H

#include <stdlib.h>

/**
 * L2 String
 * 
 * For some reason, Lineage 2 string characters are composed by two bytes.
 * The first one contains the actual letter in UTF8 (I pressume) and the second
 * one "should be" the char code, although it seems we can use zero and it
 * will work just fine.
 * 
 * (franco.montenegro)
 * Investigate why Lineage 2 uses two chars per letter
 * Investigate if Lineage 2 strings are indeed utf8
 * Investigate the proper value to set the second byte (instead of using 0)
 */

typedef char l2_string;

void l2_string_to_char(l2_string* src, char* dest, size_t size);

/**
 * Copy size amount of bytes from src to Lineage 2 string dest
 * Destination must be big enough to hold the content.
 * Use l2_string_calculate_space_from_char(size) to allocate enough 
 * space for dest
 */
void l2_string_from_char(l2_string* dest, char* src, size_t size);

/**
 * Copy size amount of bytes from Lineage 2 string to another 
 * Lineage 2 string (useful for getting Lineage 2 string from packets)
 */
void l2_string_from_l2(l2_string* dest, l2_string* src, size_t size);

/**
 * Calculate Lineage 2 string from char string
 */
size_t l2_string_calculate_space_from_char(size_t original_string);

#endif
