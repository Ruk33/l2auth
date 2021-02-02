#ifndef LIB_STRING_H
#define LIB_STRING_H

#include <stddef.h>

/**
 * Simple struct helper to avoid
 * having to type two variables
 * every time we have to deal
 * with a string.
 *
 * size refers to the size of the
 * buffer, not the length of the string.
 */
struct String {
        char *buf;
        size_t size;
};

typedef struct String string_t;

#endif
