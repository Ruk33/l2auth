#ifndef STR_H
#define STR_H

#include <stddef.h> // size_t

size_t str_memcpy(void *dest, void *src, size_t n);
size_t str_ncpy(char *dest, char *src, size_t n);
size_t str_memset(void *dest, unsigned char value, size_t n);
int str_ncmp(char *a, char *b, size_t n);

#endif
