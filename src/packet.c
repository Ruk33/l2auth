#include <assert.h>

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
 * padded = macro_packet_padded_size(packet size + checksum) = 16
 *
 * padded (16) is now safe to use as packet size.
 */
#define macro_packet_padded_size(size) \
    (((size_t) (size + 4 + 7)) & ((size_t) (~7)))

#define macro_packet_append(dest, src) \
    packet_append_n((packet_t *) dest, (byte_t *) (src), sizeof(src))

#define macro_packet_append_val(dest, src) \
    packet_append_n((packet_t *) dest, (byte_t *) &(src), sizeof(src))

typedef byte_t packet_t;

// Get packet full size (including headers).
u16_t packet_size(packet_t *src)
{
    u16_t size = 0;
    assert(src);
    size = *((u16_t *) src);
    return size;
}

u8_t packet_type(packet_t *src)
{
    assert(src);
    return (u8_t) src[2];
}

// Get pointer to packet body.
packet_t *packet_body(packet_t *src)
{
    assert(src);
    return src + 2;
}

// Append n bytes from src to dest.
void packet_append_n(packet_t *dest, byte_t *src, size_t n)
{
    u16_t size     = 0;
    u16_t new_size = 0;

    assert(dest);
    assert(src);

    size     = packet_size(dest);
    size     = size == 0 ? 2 : size; // Leave space for packet size header.
    new_size = size + n;

    *((u16_t *) dest) = new_size;

    // (franco.montenegro) Make SURE we don't overflow!
    util_cpy_bytes(dest + size, src, n, n, n);
}
