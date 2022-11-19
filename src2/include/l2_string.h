#ifndef L2_STRING_H
#define L2_STRING_H

#include "l2auth.h"

#define l2_string_to_char_arr(dest, src) \
    l2_string_to_char(as(char *, (dest)), as(l2_string *, (src)), sizeof(dest))

#define l2_string_from_char_arr(dest, src) \
    l2_string_from_char((dest), (src), sizeof(src))

void l2_string_from_char(l2_string *dest, char *src, size_t n);
void l2_string_to_char(char *dest, l2_string *src, size_t n);
size_t l2_string_len(l2_string *src, size_t n);
size_t l2_string_size(l2_string *src, size_t n);

#endif