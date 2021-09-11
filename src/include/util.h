#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>

typedef unsigned char byte_t;

typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;

#define kb(x) ((x) *1024)

#define arr_size(a) (sizeof(a) / sizeof((a)[0]))

#define log_normal(...)                                                      \
        {                                                                    \
                time_t timer       = 0;                                      \
                char buffer[32]    = { 0 };                                  \
                struct tm *tm_info = 0;                                      \
                timer              = time(0);                                \
                tm_info            = localtime(&timer);                      \
                strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", tm_info); \
                printf("[%s] ", buffer);                                     \
                printf(__VA_ARGS__);                                         \
                printf("\n");                                                \
                fflush(stdout);                                              \
        }

// Returns min number between a and b
#define _min(a, b) ((a) > (b) ? (b) : (a))

// Ensures x is higher/equal than low and lower/equal than high.
#define clamp(x, low, high) \
        (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

// Copy n bytes from src to dest and advance src.
#define byte_read_n(dest, src, n)                                  \
        do {                                                       \
                bytes_cpy((byte_t *) (dest), (byte_t *) (src), n); \
                src += n;                                          \
        } while (0)

#define byte_read(dest, src) byte_read_n(dest, src, sizeof(dest))

#define byte_read_val(dest, src) byte_read_n(&(dest), src, sizeof(dest))

#define bytes_cpy_str(dest, src, n)                                         \
        do {                                                                \
                bytes_cpy_until((byte_t *) (dest), (byte_t *) (src), 0, n); \
                (dest)[n - 1] = 0;                                          \
        } while (0);

// Copy n bytes from src to dest.
void bytes_cpy(byte_t *dest, byte_t *src, size_t n);

// Copy up to n bytes or c byte from src to dest.
void bytes_cpy_until(byte_t *dest, byte_t *src, byte_t c, size_t n);

// Zero n bytes from dest.
void bytes_zero(byte_t *dest, size_t n);

// Decode little endian.
u32_t decode32le(byte_t *buf);

// Decode big endian.
u32_t decode32be(byte_t *buf);

// Encode little endian.
void encode32le(byte_t *buf, u32_t val);

// Encode big endian.
void encode32be(byte_t *buf, u32_t val);

// Returns a usable id/index from an array of instances.
//
// Example:
// size_t instances[30] = { 0 };
// struct foo[30] = { 0 };
// size_t foo_count = 0;
//
// size_t instance = 0;
//
// foo_count += recycled_id(&instance, instances);
// foo[instance] = free instance to be used.
// recycle_id(instances, instance);
int recycle_id_get(size_t *dest, size_t *instances);

// Mark id/index to be reusable/recycled.
// Next time recycle_id_get is called, the
// recycled instances will be returned.
void recycle_id(size_t *instances, size_t id);

#endif
