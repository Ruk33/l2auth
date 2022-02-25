#ifndef INCLUDE_L2_STRING_H
#define INCLUDE_L2_STRING_H

#include <stddef.h>
#include "util.h"

typedef unsigned char l2_string_t;

#define macro_l2_str_to_char_arr(dest, src, sn) \
    l2_string_to_char(&util_macro_buf_from_arr(dest), &((struct buffer) { .buf = src, .size = sn, .used = sn }), sn)

#define macro_l2_str_arr_to_char_arr(dest, src) \
    l2_string_to_char(&util_macro_buf_from_arr(dest), &util_macro_buf_from_arr(src), sizeof(src))

#define macro_l2_str_arr_from_char_arr(dest, src) \
    l2_string_from_char(&util_macro_buf_from_arr(dest), &util_macro_buf_from_arr(src), sizeof(src))

// Copy n bytes or until NULL terminator from src to dest.
// Won't overflow and guarantees NULL terminator.
void l2_string_cpy(struct buffer *dest, struct buffer *src, size_t n);

// Convert n bytes or until NULL terminator of l2_string to char.
// Won't overflow and guarantees NULL terminator.
void l2_string_to_char(struct buffer *dest, struct buffer *src, size_t n);

// Convert n bytes or until NULL terminator of char to l2_string_t.
void l2_string_from_char(struct buffer *dest, struct buffer *src, size_t n);

size_t l2_string_len(struct buffer *src);

// Get bytes used for string (including NULL terminator).
size_t l2_string_bytes_from_buf(struct buffer *src);

size_t l2_string_bytes(l2_string_t *src, size_t n);

#endif