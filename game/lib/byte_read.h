#ifndef LIB_BYTE_READ_H
#define LIB_BYTE_READ_H

#include <string.h>

/**
 * Read n amount of bytes from src
 * and store the content in dest.
 * After reading, advance src n
 * bytes.
 *
 * Example:
 * unsigned char bytes[] = {0xff, 0xff};
 * unsigned short foo = 0;
 *
 * BYTE_READ(&foo, bytes, sizeof(foo));
 */
#define BYTE_READ(dest, src, n) memcpy(dest, src, n); src += n;

/**
 * Helper to read simple values.
 *
 * Example:
 * unsigned char bytes[] = {0xff, 0xff};
 * unsigned short foo = 0;
 *
 * BYTE_READ_VAL(foo, bytes);
 */
#define BYTE_READ_VAL(dest, src) BYTE_READ(&dest, src, sizeof(dest))

#endif
