#ifndef INCLUDE_PACKET_H
#define INCLUDE_PACKET_H

#include "util.h"

#define packet_append(dest, src) packet_append_n(dest, src, sizeof(src))

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
