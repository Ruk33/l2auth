#include "include/str.h"

size_t str_memcpy(void *dest, void *src, size_t n)
{
    if (!dest)
        return 0;
    if (!src)
        return 0;
    size_t result = 0;
    while (result < n) {
        *(((unsigned char *) dest) + result) = *(((unsigned char *) src) + result);
        result++;
    }
    return result;
}

size_t str_ncpy(char *dest, char *src, size_t n)
{
    if (!dest)
        return 0;
    if (!src)
        return 0;
    size_t result = 0;
    while (result < n) {
        // check for null terminator.
        if (!*(src+result))
            break;
        *(dest + result) = *(src + result);
        result++;
    }
    return result;
}

size_t str_memset(void *dest, unsigned char value, size_t n)
{
    if (!dest)
        return 0;
    size_t result = 0;
    while (result < n) {
        *(((unsigned char *) dest) + result) = value;
        result++;
    }
    return result;
}

int str_ncmp(char *a, char *b, size_t n)
{
    if (!a)
        return 0;
    if (!b)
        return 0;
    for (size_t i = 0; i < n; i++)
        if (*(a + i) != *(b + i))
            return 0;
    return 1;
}
