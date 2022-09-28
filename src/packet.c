#include <assert.h>
#include "include/packet.h"

u16 packet_size(struct packet *src)
{
	assert(src);
	u16 result = *((u16 *) src->buf);
	return result;
}

u16 packet_padded_size(struct packet *src)
{
	assert(src);
	// +2, two bytes used for packet size
	// +4, checksum
	// +7 & ~7 (make the entire packet multiple of 8)
	u16 result = ((packet_size(src) + 4 + 7) & (~7)) + 2;
	return result;
}

u8 packet_type(struct packet *src)
{
	assert(src);
	// 0 & 1 = packet size
	// 2 = packet type
	u8 result = src->buf[2];
	return result;
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
	// 2 first bytes are packet size (u16)
	// Body begins from byte 2
	byte *result = src->buf + 2;
	return result;
}

int packet_add_checksum(struct packet *src)
{
	u32 chksum = 0;
	u16 count = packet_size(src) - 2;
	byte *data = packet_body(src);
	u16 i = 0;
	for (i = 0; i < count; i += 4) {
		u32 ecx = data[i] & 0xff;
		ecx |= (data[i + 1] << 8) & 0xff00;
		ecx |= (data[i + 2] << 0x10) & 0xff0000;
		ecx |= (data[i + 3] << 0x18) & 0xff000000;
		chksum ^= ecx;
	}

	u32 ecx = data[i] & 0xff;
	ecx |= (data[i + 1] << 8) & 0xff00;
	ecx |= (data[i + 2] << 0x10) & 0xff0000;
	ecx |= (data[i + 3] << 0x18) & 0xff000000;

	data[i] = chksum & 0xff;
	data[i + 1] = (chksum >>0x08) & 0xff;
	data[i + 2] = (chksum >>0x10) & 0xff;
	data[i + 3] = (chksum >>0x18) & 0xff;

	*((u16 *) src->buf) = packet_size(src) + 4;

	int result = ecx == chksum;
	return result;
}

void packet_write(struct packet *dest, void *src, size_t n)
{
	u16 dest_size = 0;
	byte *tail = 0;

	assert(dest);
	assert(src);

	dest_size = packet_size(dest);
	// If the packet is empty, leave space
	// for the 2 bytes used as the length
	// of the packet and the byte used for
	// the type.
	dest_size = MAX(1, dest_size);
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
