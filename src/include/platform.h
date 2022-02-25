#ifndef INCLUDE_PLATFORM_H
#define INCLUDE_PLATFORM_H

#include <stdint.h>

typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;

typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;

typedef u8_t byte_t;

struct platform_socket;

int platform_ip_to_u32(unsigned int *dest, char *ip);

#endif
