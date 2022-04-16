#include <assert.h>
#include "include/util.h"

void cpy_str(i8 *dest, i8 *src, size_t n)
{
    assert(dest);
    assert(src);
    while (n && *src) {
        *dest = *src;
        dest += 1;
        src += 1;
        n -= 1;
    }
    // null terminator.
    *dest = 0;
}