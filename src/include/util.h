#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

#include <stdint.h>
#include <stdio.h>

#define log_normal(...)              \
        {                            \
                printf(__VA_ARGS__); \
                printf("\n");        \
        }

#define UTIL_KB(x) ((x) *1024)

#define UTIL_MB(x) (UTIL_KB(x) * 1024)

#define UTIL_GB(x) (UTIL_MB(x) * 1024)

/**
 * Get array length.
 *
 * Example:
 * u16_t i[32];
 * UTIL_ARRAY_LEN(i); // => 32
 */
#define UTIL_ARRAY_LEN(src) (sizeof(src) / sizeof(*(src)))

/**
 * Set array buffer to 0.
 *
 * Example:
 * byte_t buf[32];
 * UTIL_SET_ZERO_ARRAY(buf); // All 32 bytes in buf are 0.
 */
#define UTIL_SET_ZERO_ARRAY(dest) (util_set_zero(dest, sizeof(dest)))

/**
 * Example:
 * byte_t dest[32];
 * byte_t *src = "42";
 * UTIL_CPY_SRC_BYTES_TO_ARRAY(dest, src, 2);
 */
#define UTIL_CPY_SRC_BYTES_TO_ARRAY(dest, src, sn) \
        (util_cpy_bytes(dest, src, sizeof(dest), (u64_t)(sn), (u64_t)(sn)))

/**
 * Copy array string from src to array dest.
 *
 * Example:
 * char from[] = "lorem ipsum";
 * char to[32] = { 0 };
 * UTIL_CPY_STR_ARRAY(to, from);
 *
 * NOTE: Make sure both buffers are arrays and not just pointers!
 */
#define UTIL_CPY_STR_ARRAY(dest, src) \
        (util_cpy_str(dest, src, sizeof(dest), sizeof(src)))

#define UTIL_MIN(a, b) ((a) < (b) ? (a) : (b))

#define UTIL_MAX(a, b) ((a) > (b) ? (a) : (b))

#define UTIL_ABS(x) ((x) < 0 ? -(x) : (x))

#define UTIL_DIFF(a, b) (UTIL_ABS((a) - (b)))

#define UTIL_IS_NAN(x) ((x) != (x))

#define UTIL_IS_ODD(x) ((x) &1)

#define UTIL_IS_EVEN(x) (!UTIL_IS_ODD(x))

/**
 * Make sure a value is between min and max.
 *
 * Example:
 * UTIL_CLAMP(5, 10, 20); // Result is 10.
 * UTIL_CLAMP(15, 10, 20); // Result is 15.
 * UTIL_CLAMP(30, 10, 20); // Result is 20.
 */
#define UTIL_CLAMP(x, min, max) \
        (((x) > (max)) ? (max) : (((x) < (min)) ? (min) : (x)))

#define UTIL_SAFE_U8(x) (UTIL_MIN(x, UINT8_MAX))

#define UTIL_SAFE_U16(x) (UTIL_MIN(x, UINT16_MAX))

#define UTIL_SAFE_U32(x) (UTIL_MIN(x, UINT32_MAX))

#define UTIL_SAFE_U64(x) (UTIL_MIN(x, UINT64_MAX))

#define UTIL_SAFE_I8(x) (UTIL_CLAMP(x, INT8_MIN, INT8_MAX))

#define UTIL_SAFE_I16(x) (UTIL_CLAMP(x, INT16_MIN, INT16_MAX))

#define UTIL_SAFE_I32(x) (UTIL_CLAMP(x, INT32_MIN, INT32_MAX))

#define UTIL_SAFE_I64(x) (UTIL_CLAMP(x, INT64_MIN, INT64_MAX))

#define UTIL_PI (3.14159265)

#define UTIL_RAD_TO_DEG(x) ((x) / PI * 180)

#define UTIL_DEG_TO_RAD(x) ((x) *PI / 180)

#define UTIL_BIT(x) (1 << (x))

#define UTIL_SET_BIT(x, value) ((x) | UTIL_BIT(value))

#define UTIL_CLEAR_BIT(x, value) ((x) & ~UTIL_BIT(value))

#define UTIL_GET_BIT(x, value) (((x) >> (value)) & 1)

#define UTIL_TOGGLE_BIT(x, value) ((x) ^ UTIL_BIT(value))

#define UTIL_READ_BYTES_VAL(dest, src, n) \
        (util_read_bytes(                 \
                &(dest),                  \
                (void **) (src),          \
                sizeof(dest),             \
                UTIL_MAX(0, n),           \
                sizeof(dest)))

/**
 * Iterate through all items in a list.
 *
 * Example:
 * struct list foo[32] = { 0 };
 * int bar = 42;
 * util_list_add(foo, &bar);
 * UTIL_LIST_EACH(foo, int, value) { *value; // => 42 }
 */
#define UTIL_LIST_EACH(arr, type, name)                               \
        for (struct list *list_iterator = (arr)->head;                \
             list_iterator && (name = (type *) list_iterator->value); \
             list_iterator = list_iterator->next)

typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;

typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;

typedef u8_t byte_t;

struct list {
        struct list *head;
        struct list *next;
        void *value;
};

/**
 * Set n bytes from dest to 0.
 */
void util_set_zero(void *dest, u64_t n);

/**
 * Safely copy n bytes from src to dest making sure not to
 * overflow. The amount of bytes to copy won't exceed dn nor
 * sn.
 */
void util_cpy_bytes(void *dest, void *src, u64_t dn, u64_t sn, u64_t n);

/**
 * Copy all n bytes from src to dest or none at all if buffers
 * aren't big enough.
 * On success, 1 will be returned, otherwise, 0.
 */
int util_cpy_all_bytes(void *dest, void *src, u64_t dn, u64_t sn, u64_t n);

/**
 * Copy string from src to dest without overflowing.
 * The last character in dest is set to 0.
 */
void util_cpy_str(char *dest, char *src, u64_t dn, u64_t sn);

/**
 * Read all n bytes from src and copy to dest.
 * If buffers aren't big enough, no copy is made.
 * On success, 1 will be returned and src will be advanced n
 * bytes forward. On error, 0 will be returned and no bytes
 * will be read.
 */
int util_read_bytes(void *dest, void **src, u64_t dest_n, u64_t src_n, u64_t n);

/**
 * Add value to list contained in src buf.
 * On success, 1 is returned.
 * If there is no more space in src buf, the value won't be
 * added and 0 is returned.
 *
 * Example:
 * struct list foo[32] = { 0 };
 * int bar = 42;
 * util_list_add(foo, UTIL_ARRAY_LEN(foo), &bar);
 */
int util_list_add(struct list *src, u64_t src_len, void *value);

/**
 * Remove value from list contained in src buf.
 * If the element was found and removed, 1 is returned.
 * If the element isn't found, 0 will be returned.
 */
int util_list_remove(struct list *src, void *value);

/**
 * Decode little endian from src with n being the buf size.
 * On success, 1 will be returned and the decoded value will be
 * stored in dest. On error, 0 will be returned (buf isn't big
 * enough)
 */
int util_decode32le(u32_t *dest, byte_t *src, u64_t n);

/**
 * Decode big endian from src with n being the buf size.
 * On success, 1 will be returned and the decoded value will be
 * stored in dest. On error, 0 will be returned (buf isn't big
 * enough)
 */
int util_decode32be(u32_t *dest, byte_t *src, u64_t n);

/**
 * Encode little endian in dest with n being the buf size.
 * On success, 1 will be returned and src will be encoded in
 * dest. On error, 0 will be returned.
 */
int util_encode32le(byte_t *dest, u32_t src, u64_t n);

/**
 * Encode big endian in dest with n being the buf size.
 * On success, 1 will be returned and src will be encoded in
 * dest. On error, 0 will be returned.
 */
int util_encode32be(byte_t *dest, u32_t src, u64_t n);

/**
 * Returns a usable id/index from an array of instances.
 *
 * Example:
 * size_t instances[30] = { 0 };
 * struct foo[30] = { 0 };
 * size_t foo_count = 0;
 *
 * size_t instance = 0;
 *
 * foo_count += recycled_id(&instance, instances);
 * foo[instance] = free instance to be used.
 * recycle_id(instances, instance);
 */
int util_recycle_id_get(u64_t *dest, u64_t *instances);

/**
 * Mark id/index to be reusable/recycled.
 * Next time recycle_id_get is called, the
 * recycled instances will be returned.
 */
void util_recycle_id(u64_t *instances, u64_t id);

#endif
