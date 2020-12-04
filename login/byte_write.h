#ifndef BYTE_WRITE_H
#define BYTE_WRITE_H

#include <string.h>

/**
 * Write bytes to buffer and update position.
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
#define BYTE_WRITE(p, v, s) p = ((unsigned char *) ((unsigned char *) memcpy(p, v, s) + s))

/**
 * Helper to write simple values.
 *
 * Example:
 * unsigned char buf[8] = {0};
 * int foo = 42;
 *
 * BYTE_WRITE_VAL(buf, foo);
 */
#define BYTE_WRITE_VAL(p, v) BYTE_WRITE(p, &v, sizeof(v))

#endif
