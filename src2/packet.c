#include "include/l2auth.h"
#include "include/packet.h"

static void set_packet_size(struct packet *src, u16 new_size)
{
    assert(src);
    src->buf[0] = (u8) (new_size & 0xff);
    src->buf[1] = (u8) ((new_size >> 8) & 0xff);
}

static void increase_packet_size(struct packet *src, u16 how_much)
{
    assert(src);
    u16 size = packet_size(src);
    assert(size + how_much <= UINT16_MAX);
    u16 new_size = size + how_much;
    set_packet_size(src, new_size);
}

static byte *read_cursor(struct packet *src)
{
    assert(src);
    if (!src->read_cursor) {
        size_t size_header = sizeof(u16);
        size_t type_header = sizeof(u8);
        src->read_cursor = src->buf + size_header + type_header;
    }
    byte *result = src->read_cursor;
    return result;
}

static byte *write_cursor(struct packet *src)
{
    assert(src);
    u16 size = packet_size(src);
    if (!size) {
        // leave space for packet size header and packet type.
        u16 size_header = (u16) sizeof(u16);
        u16 type_header = (u16) sizeof(u8);
        size = size_header + type_header;
        set_packet_size(src, size);
    }
    byte *result = src->buf + size;
    return result;
}

u16 packet_size(struct packet *src)
{
    assert(src);
    u16 result = (
        ((src->buf[1] & 0xff) << 8) | 
        (src->buf[0] & 0xff)
    );
    return result;
}

u16 packet_padded_size(struct packet *src)
{
    assert(src);
    size_t size = (size_t) packet_size(src);
    // +7 & ~7 (make the entire packet multiple of 8)
    size_t result = ((size - 2 + 7) & (~7)) + 2;
    if (sizeof(src->buf) < result) {
        result = sizeof(src->buf);
        log(
            "warning: packet padded size bigger than buffer! "
            "it will be limited back to %ld to prevent overflows",
            result
        );
    }
    if (result > UINT16_MAX) {
        result = UINT16_MAX;
        log(
            "warning: packet padded size bigger than maximum unsigned short (%d). "
            "it will be capped to prevent errors.",
            UINT16_MAX
        );
    }
    return (u16) result;
}

u8 packet_type(struct packet *src)
{
    assert(src);
    u8 result = src->buf[2];
    return result;
}

void packet_set_type(struct packet *src, u8 type)
{
    assert(src);
    src->buf[2] = type;
}

byte *packet_body(struct packet *src)
{
    assert(src);
    // skip 2 bytes used for size header.
    byte *result = src->buf + 2;
    return result;
}

void packet_checksum(struct packet *src)
{
    assert(src);
    if (src->checksum_done)
        return;

    u16 body_size = packet_size(src);
    if (body_size < 2) {
        log("possible error: trying to perform checksum on an empty packet?");
        return;
    }
    // remove packet size header out of the checksum.
    body_size -= 2;

    byte *data = packet_body(src);
    u32 checksum = 0;
    for (u16 i = 0; i < body_size; i += 4) {
        u32 ecx = *data++ & 0xff;
        ecx |= (*data++ << 8) & 0xff00;
        ecx |= (*data++ << 0x10) & 0xff0000;
        ecx |= (*data++ << 0x18) & 0xff000000;
        checksum ^= ecx;
    }

    // u32 ecx = data[0] & 0xff;
    // ecx |= (data[1] << 8) & 0xff00;
    // ecx |= (data[2] << 0x10) & 0xff0000;
    // ecx |= (data[3] << 0x18) & 0xff000000;

    // *data++ = checksum & 0xff;
    // *data++ = (checksum >> 0x08) & 0xff;
    // *data++ = (checksum >> 0x10) & 0xff;
    // *data++ = (checksum >> 0x18) & 0xff;

    // src->checksum_done = 1;
    // u16 checksum_size = 4;
    // increase_packet_size(src, checksum_size);
    
    packet_write_u32(src, checksum);
    set_packet_size(src, packet_padded_size(src));
}

void packet_write(struct packet *dest, void *src, size_t n)
{
    assert(dest);
    assert(src);
    byte *cursor = write_cursor(dest);
    memcpy(cursor, src, n);
    increase_packet_size(dest, n);
}

void packet_write_u8(struct packet *dest, u8 src)
{
    assert(dest);
    byte *cursor = write_cursor(dest);
    *cursor = src;
    increase_packet_size(dest, sizeof(src));
}

void packet_write_u16(struct packet *dest, u16 src)
{
    assert(dest);
    byte *cursor = write_cursor(dest);
    *cursor++ = (u8) (src & 0xff);
    *cursor++ = (u8) ((src >> 8) & 0xff);
    increase_packet_size(dest, sizeof(src));
}

void packet_write_u32(struct packet *dest, u32 src)
{
    assert(dest);
    byte *cursor = write_cursor(dest);
    *cursor++ = (u8) (src & 0xff);
    *cursor++ = (u8) ((src >>  8) & 0xff);
    *cursor++ = (u8) ((src >> 16) & 0xff);
    *cursor++ = (u8) ((src >> 24) & 0xff);
    increase_packet_size(dest, sizeof(src));
}

void packet_write_i8(struct packet *dest, i8 src)
{
    assert(dest);
    // todo: check if this is fine.
    packet_write_u8(dest, (u8) src);
}

void packet_write_i16(struct packet *dest, i16 src)
{
    assert(dest);
    // todo: check if this is fine.
    packet_write_u16(dest, (u16) src);
}

void packet_write_i32(struct packet *dest, i32 src)
{
    assert(dest);
    // todo: check if this is fine.
    packet_write_u32(dest, (u32) src);
}

void packet_write_float(struct packet *dest, float src)
{
    assert(dest);
    byte *cursor = write_cursor(dest);
    // todo: check if this is fine.
    union {
        float f;
        u8 b[sizeof(src)];
    } temp = {0};
    temp.f = src;
    memcpy(cursor, temp.b, sizeof(src));
    increase_packet_size(dest, sizeof(src));
}

void packet_write_double(struct packet *dest, double src)
{
    assert(dest);
    byte *cursor = write_cursor(dest);
    // todo: check if this is fine.
    union {
        double d;
        u8 b[sizeof(src)];
    } temp = {0};
    temp.d = src;
    memcpy(cursor, temp.b, sizeof(src));
    increase_packet_size(dest, sizeof(src));
}

void packet_read(void *dest, struct packet *src, size_t n)
{
    assert(dest);
    assert(src);
    byte *cursor = read_cursor(src);
    memcpy(dest, cursor, n);
    src->read_cursor += n;
}

void packet_read_u8(u8 *dest, struct packet *src)
{
    assert(dest);
    assert(src);
    byte *cursor = read_cursor(src);
    u8 result = cursor[0];
    *dest = result;
    src->read_cursor += sizeof(dest);
}

void packet_read_u16(u16 *dest, struct packet *src)
{
    assert(dest);
    assert(src);
    byte *cursor = read_cursor(src);
    u16 result = ((cursor[1] & 0xff) << 8) | (cursor[0] & 0xff);
    *dest = result;
    src->read_cursor += sizeof(dest);
}

void packet_read_u32(u32 *dest, struct packet *src)
{
    assert(dest);
    assert(src);
    byte *cursor = read_cursor(src);
    u16 result = (
        ((cursor[3] & 0xff) << 8)  |
        ((cursor[2] & 0xff) << 16) |
        ((cursor[1] & 0xff) << 24) |
        ((cursor[0] & 0xff))
    );
    *dest = result;
    src->read_cursor += sizeof(dest);
}

void packet_read_i8(i8 *dest, struct packet *src)
{
    assert(dest);
    assert(src);
    // todo: check if this is fine.
    packet_read_u8((u8 *) dest, src);
}

void packet_read_i16(i16 *dest, struct packet *src)
{
    assert(dest);
    assert(src);
    // todo: check if this is fine.
    packet_read_u16((u16 *) dest, src);
}

void packet_read_i32(i32 *dest, struct packet *src)
{
    assert(dest);
    assert(src);
    // todo: check if this is fine.
    packet_read_u32((u32 *) dest, src);
}

void packet_read_float(float *dest, struct packet *src)
{
    byte *cursor = read_cursor(src);
    // todo: check if this is fine.
    union {
        float result;
        u8 buf[sizeof(*dest)];
    } temp = {0};
    memcpy(temp.buf, cursor, sizeof(*dest));
    *dest = temp.result;
    src->read_cursor += sizeof(*dest);
}

void packet_read_double(double *dest, struct packet *src)
{
    byte *cursor = read_cursor(src);
    // todo: check if this is fine.
    union {
        double result;
        u8 buf[sizeof(*dest)];
    } temp = {0};
    memcpy(temp.buf, cursor, sizeof(*dest));
    *dest = temp.result;
    src->read_cursor += sizeof(*dest);
}
