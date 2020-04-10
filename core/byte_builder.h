#ifndef L2AUTH_BYTE_BUILDER_H
#define L2AUTH_BYTE_BUILDER_H

#include <stdlib.h>

/**
 * Byte builder
 * 
 * Similar to byte_buffer but instead of relying on malloc you can
 * work on a preallocated chunk of memory.
 */

typedef unsigned char byte_builder;

/**
 * Byte builder uses some of the memory in order to save
 * how much memory was allocated for the buffer and the
 * length of it
 */
size_t byte_builder_calculate_size(size_t to_be_allocated);
/**
 * Before using the byte builder make sure to initialize it first
 */
byte_builder* byte_builder_init(byte_builder* builder, size_t allocated);
size_t byte_builder_length(byte_builder* builder);
void byte_builder_append
(
        byte_builder* builder,
        unsigned char* content,
        size_t content_size
);

#endif
