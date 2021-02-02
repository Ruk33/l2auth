#ifndef LIB_L2_STRING_H
#define LIB_L2_STRING_H

#include <stddef.h>

enum L2StringMessageType {
        SAY_MSG_TYPE_NORMAL,
        SAY_MSG_TYPE_SHOUT,
        SAY_MSG_TYPE_WHISPER,
        SAY_MSG_TYPE_PARTY,
        SAY_MSG_TYPE_CLAN,
        SAY_MSG_TYPE_GM,
};

typedef enum L2StringMessageType l2_string_type_t;

typedef unsigned char l2_string_t;

/**
 * Write n amount of bytes from
 * Lineage 2 string stored in src
 * to dest.
 */
void l2_string_to_char(char *dest, l2_string_t *src, size_t n);

/**
 * Convert n bytes of string into Lineage 2 string.
 */
void l2_string_from_char(l2_string_t *dest, char *src, size_t n);

/**
 * Return length from Lineage 2 string
 * stored in src length.
 */
size_t l2_string_len(l2_string_t *src);

/**
 * Return bytes used by Lineage 2
 * string stored in src.
 */
size_t l2_string_bytes(l2_string_t *src);

#endif
