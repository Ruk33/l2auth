#include <stddef.h>
#include "include/l2_string.h"

void l2_string_cpy(l2_string_t *dest,
                   l2_string_t *src,
                   size_t dn,
                   size_t sn,
                   size_t n)
{
    if (!dest || !src || !dn || !sn || !n) {
        return;
    }

    while (dn >= 2 && sn >= 2 && n >= 2 && *src) {
        *dest++ = *src++;
        *dest++ = *src++;

        dn -= 2;
        sn -= 2;
        n -= 2;
    }

    *dest = 0;

    // Try to add the secondary byte to the l2 string.
    // Todo: make this code better, maybe we can find a cleaner way?
    if (dn) {
        *(dest + 1) = 0;
    }
}

void l2_string_to_char(char *dest,
                       l2_string_t *src,
                       size_t dn,
                       size_t sn,
                       size_t n)
{
    if (!dest || !src || !dn || !sn || !n) {
        return;
    }

    while (dn && sn >= 2 && n && *src) {
        *dest++ = (char) *src++;

        // Ignore the second byte of each l2 string character.
        src++;

        dn -= 1;
        sn -= 2;
        n -= 1;
    }

    *dest = 0;
}

void l2_string_from_char(l2_string_t *dest,
                         char *src,
                         size_t dn,
                         size_t sn,
                         size_t n)
{
    if (!dest || !src || !dn || !sn || !n) {
        return;
    }

    while (dn >= 2 && sn && n && *src) {
        *dest++ = (unsigned char) *src++;
        *dest++ = 0;

        dn -= 2;
        sn -= 1;
        n -= 1;
    }

    *dest = 0;

    // Try to add the secondary byte to the l2 string.
    // Todo: make this code better, maybe we can find a cleaner way?
    if (dn) {
        *(dest + 1) = 0;
    }
}

size_t l2_string_len(l2_string_t *src, size_t n)
{
    size_t len = 0;

    if (!src || !n) {
        return 0;
    }

    while (n >= 2 && *src) {
        len += 1;
        src += 2;
        n -= 2;
    }

    return len;
}

size_t l2_string_bytes(l2_string_t *src, size_t n)
{
    if (!src || !n) {
        return 0;
    }
    return (l2_string_len(src, n) + 1) * 2;
}
