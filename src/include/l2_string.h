#ifndef INCLUDE_L2_STRING_H
#define INCLUDE_L2_STRING_H

#include "util.h"

typedef unsigned char l2_string_t;

#define L2_STRING_TO_CHAR_ARRAY(dest, src, sn) \
    l2_string_to_char((char *) dest,           \
                      (l2_string_t *) src,     \
                      sizeof(dest),            \
                      (size_t) sn,             \
                      (size_t) sn)

#define L2_STRING_ARRAY_TO_CHAR_ARRAY(dest, src) \
    l2_string_to_char((char *) dest,             \
                      (l2_string_t *) src,       \
                      sizeof(dest),              \
                      sizeof(src),               \
                      sizeof(src))

#define L2_STRING_ARRAY_FROM_CHAR_ARRAY(dest, src) \
    l2_string_from_char((l2_string_t *) dest,      \
                        (char *) src,              \
                        sizeof(dest),              \
                        sizeof(src),               \
                        sizeof(src))

// Copy n bytes or until NULL terminator from src to dest.
// Won't overflow and guarantees NULL terminator.
void l2_string_cpy(l2_string_t *dest,
                   l2_string_t *src,
                   size_t dn,
                   size_t sn,
                   size_t n);

// Convert n bytes or until NULL terminator of l2_string to char.
// Won't overflow and guarantees NULL terminator.
void l2_string_to_char(char *dest,
                       l2_string_t *src,
                       size_t dn,
                       size_t sn,
                       size_t n);

// Convert n bytes or until NULL terminator of char to l2_string_t.
void l2_string_from_char(l2_string_t *dest,
                         char *src,
                         size_t dn,
                         size_t sn,
                         size_t n);

// Get length of string.
size_t l2_string_len(l2_string_t *src, size_t n);

// Get bytes used for string (including NULL terminator).
size_t l2_string_bytes(l2_string_t *src, size_t n);

#endif
