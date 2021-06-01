#ifndef INCLUDE_L2_STRING_H
#define INCLUDE_L2_STRING_H

#include "util.h"

typedef char l2_string_t;

// Copy n bytes or until NULL terminator from src to dest.
void l2_string_cpy(l2_string_t *dest, l2_string_t *src, size_t n);

// Convert n bytes or until NULL terminator of l2_string to char.
void l2_string_to_char(char *dest, l2_string_t *src, size_t n);

// Convert n bytes or until NULL terminator of char to l2_string_t.
void l2_string_from_char(l2_string_t *dest, char *src, size_t n);

// Get length of string.
size_t l2_string_len(l2_string_t *src);

// Get bytes used for string (including NULL terminator).
size_t l2_string_bytes(l2_string_t *src);

#endif
