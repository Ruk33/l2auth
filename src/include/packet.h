#ifndef PACKET_H
#define PACKET_H

#include "util.h"

struct packet {
    // 2 bytes for packet size
    // 1 byte for packet type
    // Rest of the bytes for packet content
    byte buf[65535];
};

// Get the full size of the packet which includes:
// - 1 byte for packet type
// - bytes used for the rest of the packet.
u16 packet_size(struct packet *src);

// Get padded size so packet size is multiple of 8.
// This does not represent the real size of the packet
// and should only be used to send the packet
// back to the client.
u16 packet_padded_size(struct packet *src);

// Get the packet type.
u8 packet_type(struct packet *src);

// Set the packet type.
void packet_set_type(struct packet *src, u8 type);

// Get the pointer to where the body of
// the packet begins (which is in the
// type's byte)
byte *packet_body(struct packet *src);

// Append n bytes to the end of the packet.
void packet_body_append(struct packet *dest, void *src, size_t n);

// Append u8 to body.
void packet_body_u8(struct packet *dest, u8 src);

// Append u16 to body.
void packet_body_u16(struct packet *dest, u16 src);

// Append u32 to body.
void packet_body_u32(struct packet *dest, u32 src);

// Append i8 to body.
void packet_body_i8(struct packet *dest, i8 src);

// Append i16 to body.
void packet_body_i16(struct packet *dest, i16 src);

// Append i32 to body.
void packet_body_i32(struct packet *dest, i32 src);

#endif