#include "include/util.h"
#include "include/l2_string.h"

void l2_string_cpy(l2_string_t *dest, l2_string_t *src, size_t n)
{
        while (dest && src && *src && n) {
                *dest       = *src;
                *(dest + 1) = *(src + 1);

                dest += 2;
                src += 2;
                n -= 2;
        }
}

void l2_string_to_char(char *dest, l2_string_t *src, size_t n)
{
        while (dest && src && *src && n) {
                *dest = *src;
                dest += 1;
                src += 2;
                n -= 1;
        }
}

void l2_string_from_char(l2_string_t *dest, char *src, size_t n)
{
        while (dest && src && *src && n) {
                *dest       = *src;
                *(dest + 1) = 0;

                dest += 2;
                src += 1;
                n -= 1;
        }
}

size_t l2_string_len(l2_string_t *src)
{
        size_t len = 0;

        while (src && *src) {
                len += 1;
                src += 2;
        }

        return len;
}

size_t l2_string_bytes(l2_string_t *src)
{
        return (l2_string_len(src) + 1) * 2;
}
