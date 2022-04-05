#ifndef UTIL_H
#define UTIL_H

#include <stddef.h> // size_t
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

typedef u8 byte;

// Buffer from fixed array (ie, char[32])
#define BFA(arr) ((struct buffer) { .buf = (arr), .size = sizeof(arr), .used = sizeof(arr) })
// Buffer from value (ie, int)
#define BFV(val) ((struct buffer) { .buf = &(val), .size = sizeof(val), .used = sizeof(val) })
// Buffer
#define B(p, s, u) ((struct buffer) { .buf = (p), .size = (s), .used = (u) })

struct buffer {
    void *buf;
    size_t size;
    size_t used;
};

// Encode/decode big/little endian.
// Buffers (src & dest) must be at least
// 4 bytes to avoid overflows.
//
// Decode little endian from src to dest.
void decode32le(u32 *dest, void *src);
// Decode big endian from src to dest.
void decode32be(u32 *dest, void *src);
// Encode little endian to dest.
void encode32le(void *dest, u32 src);
// Encode big endian to dest.
void encode32be(void *dest, u32 src);

#endif