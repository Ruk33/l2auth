#ifndef L2AUTH_BYTE_READER_H
#define L2AUTH_BYTE_READER_H

#include <stdlib.h>

/**
 * Copy to_copy bytes from src into dest and move cursor
 * Useful to read data from packets
 * 
 * Example
 * l2_packet* some_packet = ... // Assume packet is |int|int|
 * int a = 0;
 * int b = 0;
 * some_packet = byte_reader_cpy_n_mv(some_packet, &a, sizeof(a));
 * some_packet = byte_reader_cpy_n_mv(some_packet, &b, sizeof(b));
 */
void* byte_reader_cpy_n_mv
(
        void* src,
        void* dest, 
        size_t to_copy
);

void* byte_reader_cpy_int_n_mv(void* src, void* dest);

void* byte_reader_cpy_short_n_mv(void* src, void* dest);

void* byte_reader_cpy_char_n_mv(void* src, void* dest);

#endif
