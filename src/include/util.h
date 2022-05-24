#ifndef UTIL_H
#define UTIL_H

#include <stddef.h> // size_t
#include <stdint.h> // fixed int types
#include <stdio.h>  // printf

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

typedef u8 byte;

typedef float seconds;

#define TODO(...)							\
	do {								\
		printf("\nTODO (%s line %d)\n", __FILE__, __LINE__);	\
		printf(__VA_ARGS__);					\
		printf("\n");						\
	} while (0)

#define KB(x) ((x) * 1024)

#define ARR_LEN(arr) (sizeof(arr) / sizeof(*(arr)))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define CPY_STR(dest_arr, src) (cpy_str((i8 *)(dest_arr), (i8 *)(src), sizeof(dest_arr)))

struct ipv4 {
	// Maximum, 255.255.255.255, 15 + NULL terminator = 16, quick math.
	char buf[16];
};

// Copy up to n bytes or null terminator.
// Destination is guarantee to end up with a null terminator.
void cpy_str(i8 *dest, i8 *src, size_t n);

void cpy_bytes(byte *dest, byte *src, size_t n);

// We can't use endian.h in Windows (apparently)
// so let's go ahead and implement these for
// the sake of simplicity.
u32 be32_to_le(u32 src);
u32 le32_to_be(u32 src);

// Convert text representation of IPv4 to u32.
// On success, 1 is returned.
int ipv4_to_u32(u32 *dest, struct ipv4 *src);

#endif
