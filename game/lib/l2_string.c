#include <assert.h>
#include <stddef.h>
#include "l2_string.h"

void l2_string_to_char(char *dest, l2_string *src, size_t n)
{
        assert(dest);
        assert(src);

        size_t i = 0;

        while (*src && i < n)
        {
                dest[i] = (char) *src;
                i += 1;
                src += 2;
        }

        dest[i] = 0;
}

void l2_string_from_char(l2_string *dest, char *src, size_t n)
{
        assert(dest);
        assert(src);
        assert(n);

        for(size_t i = 0; i < n; i++)
        {
                *dest = (unsigned char) src[i];
                *(dest + 1) = 0;
                dest += 2;
        }
}

size_t l2_string_len(l2_string *src)
{
        assert(src);

        size_t len = 0;

        while (*src)
        {
                len += 1;
                src += 2;
        }

        return len;
}

size_t l2_string_bytes(l2_string *src)
{
        assert(src);
        return (l2_string_len(src) + 1) * 2;
}
