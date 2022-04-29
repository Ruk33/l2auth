#ifndef L2_STRING_H
#define L2_STRING_H

#include "../../../include/util.h"

typedef i16 l2_string;

#define L2_STRING_FROM_CHAR(dest, src) (l2_string_from_char((l2_string *)(dest), (i8 *)(src), sizeof(dest)))

// Copy up to n bytes or null terminator from src to dest.
void l2_string_from_char(l2_string *dest, i8 *src, size_t n);

// Get length of l2 string.
size_t l2_string_len(l2_string *src);

// Get bytes used by l2 string (including null terminator)
size_t l2_string_size(l2_string *src);

#endif