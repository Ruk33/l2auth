#include <assert.h>
#include "include/packet.h"

u16 packet_size(struct packet *src)
{
    assert(src);
    // +2, bytes used to store the entire packet size.
    // +1, always consider packet type.
    return 2 + 1 + (((u16 *) src->buf)[0]);
}

u16 packet_padded_size(struct packet *src)
{
    assert(src);
    // +4, checksum
    return (packet_size(src) + 4 + 7) & (~7);
}

u8 packet_type(struct packet *src)
{
    assert(src);
    // 0 & 1 = packet size
    // 2 = packet type
    return ((u8 *) src->buf)[2];
}

void packet_set_type(struct packet *dest, u8 type)
{
    assert(dest);
    // 0 & 1 = packet size
    // 2 = packet type
    ((u8 *) dest->buf)[2] = type;
}

byte *packet_body(struct packet *src)
{
    assert(src);
    // 0 & 1 = packet size
    // Body begins from byte 2
    return src->buf + 2;
}

void packet_body_append(struct packet *dest, struct buffer *src)
{
    u16 dest_size = 0;
    byte *tail = 0;

    assert(dest);
    assert(src);
    assert(src->buf);

    // Only body size (without packet type)
    dest_size = (((u16 *) dest->buf)[0]);
    // +2, skip bytes used for packet size.
    // +1, skip byte for packet type.
    tail = dest->buf + 2 + 1;

    for (size_t i = 0; i < src->used; i += 1) {
        *tail = ((byte *) src->buf)[i];
        tail += 1;
    }

    // Set new packet body size (without packet type
    // since it's already considered by packet_size)
    ((u16 *) dest->buf)[0] = dest_size + (u16) src->used;
}

void packet_body_u8(struct packet *dest, u8 src)
{
    assert(dest);
    packet_body_append(dest, &BFV(src));
}

void packet_body_u16(struct packet *dest, u16 src)
{
    assert(dest);
    packet_body_append(dest, &BFV(src));
}

void packet_body_u32(struct packet *dest, u32 src)
{
    assert(dest);
    packet_body_append(dest, &BFV(src));
}

void packet_body_i8(struct packet *dest, i8 src)
{
    assert(dest);
    packet_body_append(dest, &BFV(src));
}

void packet_body_i16(struct packet *dest, i16 src)
{
    assert(dest);
    packet_body_append(dest, &BFV(src));
}

void packet_body_i32(struct packet *dest, i32 src)
{
    assert(dest);
    packet_body_append(dest, &BFV(src));
}