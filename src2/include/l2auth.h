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

#define as(type, x) \
    ((type) (x))

#define min(a, b) \
    ((a) < (b) ? (a) : (b))

#define arr_len(x) \
    (sizeof(x) / sizeof(*(x)))

#define for_each(type, name, arr) \
    for (type *name = arr; name < arr + arr_len(arr); name++)

#define for_each_ex(type, name, arr, limit) \
    for (type *name = arr; name < arr + min((limit), arr_len(arr)); name++)

#define kb(x) \
    ((x) * 1024)

#define str_cpy(dest, src) \
    (strncpy(as(char *, (dest)), as(char *, (src)), sizeof(dest) - 1))

#define str_matches(a, b) \
    (strncmp(as(char *, (a)), as(char *, (b)), sizeof(a) - 1) == 0)

#define zero(x) \
    (memset((x), 0, sizeof(*(x))))

#define zero_arr(x) \
    (memset((x), 0, sizeof(x)))

#define log(...)            \
    do {                    \
        printf(__VA_ARGS__);\
        printf("\n");       \
    } while (0)

#define log_err(...)                                                    \
    do {                                                                \
        log("[ERROR] - - - - - - - - - - - - - - - - - - - - - - -");   \
        log(__VA_ARGS__);                                               \
        log("- - - - - - - - - - - - - - - - - - - - - - - - - - -");   \
    } while(0)

#include "network.h"
#include "endian.h"
#include "packet.h"
#include "l2_string.h"
#include "l2_types.h"
#include "l2_random.h"
#include "storage.h"

#endif
