#ifndef UTIL_H
#define UTIL_H

#include <stddef.h> // size_t
#include <stdint.h> // fixed int types
#include <stdio.h> // printf

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

typedef u8 byte;

typedef float seconds;

#define TODO(msg) (printf("\nTODO (%s line %d)\n  %s\n\n", __FILE__, __LINE__, (msg)))

#define ARR_LEN(arr) (sizeof(arr) / sizeof(*(arr)))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define CPY_STR(dest_arr, src) (cpy_str((i8 *)(dest_arr), (i8 *)(src), sizeof(dest_arr)))

// Copy up to n bytes or null terminator.
// Destination is guarantee to end up with a null terminator.
void cpy_str(i8 *dest, i8 *src, size_t n);

#endif