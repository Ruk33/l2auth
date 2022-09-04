#include <assert.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/packet_reader.h"

void packet_read_init(struct packet_reader *reader, struct packet *packet)
{
	assert(reader);
	assert(packet);
	reader->packet = packet;
	reader->tail = packet_body(packet) + 1; // +1, ignore packet type.
}

size_t packet_read_bytes(byte *dest, struct packet_reader *src, size_t n)
{
	assert(dest);
	assert(src);

	if (src->packet->buf + packet_size(src->packet) < src->tail + n)
		return 0;

	cpy_bytes(dest, src->tail, n);
	src->tail += n;
	
	return n;
}

size_t packet_read_str(l2_string *dest, struct packet_reader *src, size_t n)
{
	assert(dest);
	assert(src);
	size_t str_size = l2_string_size((l2_string *) src->tail);
	return packet_read_bytes((byte *) dest, src, MIN(n, str_size));
}

size_t packet_read_i8(i8 *dest, struct packet_reader *src)
{
	assert(dest);
	assert(src);
	return packet_read_bytes((byte *) dest, src, sizeof(*dest));
}

size_t packet_read_i16(i16 *dest, struct packet_reader *src)
{
	assert(dest);
	assert(src);
	return packet_read_bytes((byte *) dest, src, sizeof(*dest));
}

size_t packet_read_i32(i32 *dest, struct packet_reader *src)
{
	assert(dest);
	assert(src);
	return packet_read_bytes((byte *) dest, src, sizeof(*dest));
}

size_t packet_read_u8(u8 *dest, struct packet_reader *src)
{
	assert(dest);
	assert(src);
	return packet_read_bytes((byte *) dest, src, sizeof(*dest));
}

size_t packet_read_u16(u16 *dest, struct packet_reader *src)
{
	assert(dest);
	assert(src);
	return packet_read_bytes((byte *) dest, src, sizeof(*dest));
}

size_t packet_read_u32(u32 *dest, struct packet_reader *src)
{
	assert(dest);
	assert(src);
	return packet_read_bytes((byte *) dest, src, sizeof(*dest));
}
