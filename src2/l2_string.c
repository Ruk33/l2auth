#include "include/l2auth.h"

void l2_string_from_char(l2_string *dest, char *src, size_t n)
{
    assert(dest);
    assert(src);
    while (n > 0 && *src) {
        *((i8 *) dest) = *src;
        src++;
        dest++;
        n--;
    }
}

void l2_string_to_char(char *dest, l2_string *src, size_t n)
{
    assert(dest);
    assert(src);
    while (n > 0 && *((i8 *) dest)) {
        *dest = *((i8 *) src);
        dest++;
        src++;
        n--;
    }
    // null terminator.
    *dest = 0;
}

size_t l2_string_len(l2_string *src, size_t n)
{
    assert(src);
    size_t result = 0;
    while (n > 0 && *((i8 *) src)) {
        result++;
        src++;
        n--;
    }
    return result;
}

size_t l2_string_size(l2_string *src, size_t n)
{
    assert(src);
    // +1 = null terminator.
    return (l2_string_len(src, n) + 1) * 2;
}
