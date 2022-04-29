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

void cpy_bytes(byte *dest, byte *src, size_t n)
{
    assert(dest);
    assert(src);
    while (n) {
        *dest = *src;
        dest += 1;
        src += 1;
        n -= 1;
    }
}

u32 be32tole(u32 src)
{
    return (
        ((src >> 24) & 0xff      ) |
        ((src <<  8) & 0xff0000  ) |
        ((src >>  8) & 0xff00    ) |
        ((src << 24) & 0xff000000)
    );
}

u32 le32tobe(u32 src)
{
    return (
        ((src & 0x000000ff) << 24) |
        ((src & 0x0000ff00) <<  8) |
        ((src & 0x00ff0000) >>  8) |
        ((src & 0xff000000) >> 24)
    );
}

int ipv4_to_u32(u32 *dest, struct ipv4 *src)
{
    char *buf = 0;
    u32 tmp = 0;

    assert(dest);
    assert(src);

    buf = src->buf;

    for (int i = 0; i < 4; i += 1) {
        tmp = 0;
        for (int n = 0; *buf && *buf != '.' && n < 3; n += 1) {
            // Only numbers for IP.
            if (*buf >= '0' && *buf <= '9') {
                tmp = 10 * tmp + *buf - '0';
                buf += 1;
            // Anything else, abort.
            } else {
                goto abort;
            }
        }
        ((u8 *) dest)[i] = (u8) tmp;
        if (tmp > 255) {
            goto abort;
        }
        // If the entire IP has been converted
        // just exit.
        if (i == 3) {
            break;
        }
        if (*buf != '.') {
            goto abort;
        }
        // Skip dot (.)
        buf += 1;
    }

    return 1;

abort:
    *dest = 0;
    return 0;
}
