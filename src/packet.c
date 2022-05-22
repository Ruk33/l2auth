#include <assert.h>
#include "include/packet.h"

u16 packet_size(struct packet *src)
{
	assert(src);
	return ((u16 *) src->buf)[0];
}

u16 packet_padded_size(struct packet *src)
{
	assert(src);
	// +2, bytes used for packet size header.
	// +4, checksum
	return ((packet_size(src) + 4 + 7) & (~7)) + 2;
}

u8 packet_type(struct packet *src)
{
	assert(src);
	// 0 & 1 = packet size
	// 2 = packet type
	return src->buf[2];
}

void packet_set_type(struct packet *dest, u8 type)
{
	assert(dest);
	// 0 & 1 = packet size
	// 2 = packet type
	dest->buf[2] = type;
}

byte *packet_body(struct packet *src)
{
	assert(src);
	// 0 & 1 = packet size
	// Body begins from byte 2
	return src->buf + 2;
}

void packet_write(struct packet *dest, void *src, size_t n)
{
	u16 dest_size = 0;
	byte *tail = 0;

	assert(dest);
	assert(src);

	dest_size = packet_size(dest);
	// If the packet is empty, leave space
	// for the packet type header.
	dest_size = MAX(1, dest_size);
	// +2 skip packet size header.
	tail = packet_body(dest) + dest_size;

	for (size_t i = 0; i < n; i += 1) {
		*tail = *(((byte *) src) + i);
		tail += 1;
	}

	// Set new packet size.
	*((u16 *) dest->buf) = dest_size + (u16) n;
}

void packet_write_u8(struct packet *dest, u8 src)
{
	assert(dest);
	packet_write(dest, &src, sizeof(src));
}

void packet_write_u16(struct packet *dest, u16 src)
{
	assert(dest);
	packet_write(dest, &src, sizeof(src));
}

void packet_write_u32(struct packet *dest, u32 src)
{
	assert(dest);
	packet_write(dest, &src, sizeof(src));
}

void packet_write_i8(struct packet *dest, i8 src)
{
	assert(dest);
	packet_write(dest, &src, sizeof(src));
}

void packet_write_i16(struct packet *dest, i16 src)
{
	assert(dest);
	packet_write(dest, &src, sizeof(src));
}

void packet_write_i32(struct packet *dest, i32 src)
{
	assert(dest);
	packet_write(dest, &src, sizeof(src));
}

void packet_write_d(struct packet *dest, double src)
{
	assert(dest);
	packet_write(dest, &src, sizeof(src));
}
