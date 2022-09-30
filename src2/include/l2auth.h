#ifndef L2AUTH_H
#define L2AUTH_H

#include <assert.h> // assert
#include <stddef.h> // size_t
#include <stdint.h> // fixed int types
#include <string.h> // memcpy, strncpy, memset
#include <stdio.h>  // printf

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;

typedef u8 byte;
typedef float seconds;

#define arr_len(x) \
    sizeof(x) / sizeof(*(x))

#define for_each(type, name, arr) \
    for (type *name = arr; name < arr + arr_len(arr); name++)

#define kb(x) \
    ((size_t) ((x) * 1024))

#define min(a, b) \
    ((a) < (b) ? (a) : (b))

#define str_cpy(dest, src) \
    (strncpy((char *) dest, (char *) src, sizeof(dest) - 1))

#define zero(x) \
    (memset(x, 0, sizeof(*(x))))

#define zero_arr(x) \
    (memset(x, 0, sizeof(x)))    

#define log(...)            \
    do {                    \
        printf(__VA_ARGS__);\
        printf("\n");       \
    } while (0)

// u32 randi(u32 index)
// {
//     index = (index << 13) ^ index;
//     return ((index * (index * index * 15731 + 789221) + 1376312589) & 0x7fffffff);
// }

// int ipv4_to_u32(u32 *dest, struct ipv4 *src);

#include "endian.h"
#include "packet.h"
#include "l2_string.h"
#include "login_session.h"

#include "login_request.h"
#include "login_response.h"
#include "login_server.h"

#endif
