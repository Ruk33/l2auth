#define nl "\n"

/*
 * printf alternative.
 * Write to stderr so we don't have to fflush the buffer.
 */
#define trace(...) fprintf(stderr, __VA_ARGS__)

/*
 * assert alternative.
 */
#define check(what) \
    ((what) ? (what) : (trace("assertion failed in %s:%d: %s" nl, __FILE__, __LINE__, #what), *((volatile int *) 0) = 0, (what)))

/*
 * Append src to dest and return dest + sizeof(src)
 */
#define append(dest, src) (memcpy((dest), &(src), sizeof(src)), (dest) + sizeof(src))

/*
 * In ms.
 */
#define seconds * 1000
#define second seconds
#define minutes * 60 * 1000
#define minute minutes

/*
 * In bytes.
 */
#define kb * 1024
#define mb * 1024 * 1024

#define get_memory(bytes) (calloc(1, (bytes)))

#define array_length(x) (sizeof(x) / sizeof(*(x)))

#define same_memory(a, b, size) (memcmp((a), (b), (size)) == 0)
#define copy_memory(dest, src, size) (memcpy((dest), (src), (size)))
#define move_memory(dest, src, size) (memmove((dest), (src), (size)))
#define copy_string(dest, src, size) (strncpy((dest), (src), (size)))
#define copy_string_from_format(dest, format, ...) (snprintf((dest), sizeof(dest) - 1, (format), __VA_ARGS__))
#define reset_memory(memory, size) (memset((memory), 0, (size)))

/*
 * Format:
 * config_name=value
 *
 * Example:
 * id=1
 */
#define read_config(file, config_name, format, dest) \
    (fscanf((file), config_name "=" format nl, (dest)) == 1)

#define write_config(file, config_name, format, value) \
    (fprintf((file), config_name "=" format nl, (value)))

#define coroutine(c)                        \
    struct coroutine *__coroutine = (c);    \
    switch (__coroutine->state)             \
        case 0:

#define yield                               \
    __coroutine->state = __COUNTER__ + 1;   \
    break;                                  \
    case (__COUNTER__):

#define yield_for(sleep_ms, passed_time)    \
    __coroutine->sleep = (sleep_ms);        \
    yield;                                  \
    __coroutine->sleep -= (passed_time);    \
    if (__coroutine->sleep > 0)             \
        break

typedef uint8_t byte;

typedef  uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef  int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

struct coroutine {
    int state;
    float sleep;
};

u64 string_size(char *src)
{
    char *tail = src;
    while (*tail++);
    return tail - src;
}

u64 string_length(char *src)
{
    return string_size(src) - 1;
}

int same_string(char *a, char *b)
{
    while (*a && *a++ == *b++);
    return (*a + *b) == 0;
}
