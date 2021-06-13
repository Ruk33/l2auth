#ifndef INCLUDE_PACKET_H
#define INCLUDE_PACKET_H

#include "util.h"

/**
 * Packets need to be multiple of 8
 * Not only that, we need to reserve
 * 4 bytes for checksum.
 *
 * Example:
 *
 * packet size = 5 (includes byte from packet type)
 * checksum = 4
 * packet size + checksum = 9
 * padded = PACKET_PADDED_SIZE(packet size + checksum) = 16
 *
 * padded (16) is now safe to use as packet size.
 */
#define packet_padded_size(size) (((size_t)(size + 4 + 7)) & ((size_t)(~7)))

#define packet_append(dest, src) \
        packet_append_n(dest, (byte_t *) (src), sizeof(src))

#define packet_append_val(dest, src) \
        packet_append_n(dest, (byte_t *) &(src), sizeof(src))

typedef byte_t packet_t;

// Get packet full size (including headers).
u16_t packet_size(packet_t *src);

// Get packet type.
u8_t packet_type(packet_t *src);

// Get pointer to packet body.
packet_t *packet_body(packet_t *src);

// Append n bytes from src to dest.
void packet_append_n(packet_t *dest, byte_t *src, size_t n);

#endif
