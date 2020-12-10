#ifndef LIB_BYTE_WRITE_H
#define LIB_BYTE_WRITE_H

#include <string.h>

/**
 * Write n amount of bytes from dest to src
 * and move src n bytes forward.
 *
 * Useful for appending bytes into buffer.
 *
 * Example:
 * unsigned char bytes[8] = {0};
 * unsigned char *p = bytes;
 * int buf_size = 0;
 *
 * int foo = 42;
 * int bar = 24;
 *
 * BYTE_WRITE(p, &foo, sizeof(foo));
 * BYTE_WRITE(p, &bar, sizeof(bar));
 *
 * buf_size = p - bytes;
 */
#define BYTE_WRITE(dest, src, n) dest = ((unsigned char *) ((unsigned char *) memcpy(dest, src, n) + n))

/**
 * Helper to write simple values into dest from src.
 *
 * Example:
 * unsigned char buf[8] = {0};
 * int foo = 42;
 *
 * BYTE_WRITE_VAL(buf, foo);
 */
#define BYTE_WRITE_VAL(dest, src) BYTE_WRITE(dest, &src, sizeof(src))

#endif
