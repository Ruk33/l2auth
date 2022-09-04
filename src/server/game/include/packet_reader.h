#ifndef PACKET_READER_H
#define PACKET_READER_H

#include <assert.h>
#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "l2_string.h"

struct packet_reader {
	struct packet *packet;
	byte *tail;
};

void packet_read_init(struct packet_reader *reader, struct packet *packet);

// Copy n bytes (or 0 if end of packet reached) from src to dest.
// Returns number of read bytes.
size_t packet_read_bytes(byte *dest, struct packet_reader *src, size_t n);

// Read up to n bytes or null terminator from dest to src.
size_t packet_read_str(l2_string *dest, struct packet_reader *src, size_t n);

size_t packet_read_i8(i8 *dest, struct packet_reader *src);

size_t packet_read_i16(i16 *dest, struct packet_reader *src);

size_t packet_read_i32(i32 *dest, struct packet_reader *src);

size_t packet_read_u8(u8 *dest, struct packet_reader *src);

size_t packet_read_u16(u16 *dest, struct packet_reader *src);

size_t packet_read_u32(u32 *dest, struct packet_reader *src);

#endif
