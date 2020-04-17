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
 * some_packet = byte_reader_copy_and_move(some_packet, &a, sizeof(a));
 * some_packet = byte_reader_copy_and_move(some_packet, &b, sizeof(b));
 */
void* byte_reader_copy_and_move
(
        void* src,
        void* dest, 
        size_t to_copy
);

#endif
