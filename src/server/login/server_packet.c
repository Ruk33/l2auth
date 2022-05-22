#include <assert.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/server_packet.h"

void packet_init_to(struct packet *dest, struct packet_init *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x00);
	packet_write(dest, src->session_id.buf, sizeof(src->session_id.buf));
	packet_write(dest, src->protocol.buf, sizeof(src->protocol.buf));
	packet_write(dest, src->modulus.buf, sizeof(src->modulus.buf));

	// Add additional two bytes for the packet size.
	// NOT ideal...
	packet_write_i16(dest, 0);
	TODO("don't add two extra bytes for the packet size header in the init packet");
}

void packet_gg_auth_to(struct packet *dest, struct packet_gg_auth *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x0b);
	packet_write_i32(dest, src->gg_response);
}

void packet_ok_to(struct packet *dest, struct packet_ok *src)
{
	byte after_key[] = {
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0xea, 0x03, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x02, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x60, 0x62, 0xe0, 0x00,
		0x00, 0x00, 0x00,
	};
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x03);
	packet_write_i32(dest, src->loginOK1);
	packet_write_i32(dest, src->loginOK2);
	packet_write(dest, after_key, sizeof(after_key));
}

void packet_play_ok_to(struct packet *dest, struct packet_play_ok *src)
{
	assert(dest);
	assert(src);
	packet_set_type(dest, 0x07);
	packet_write_i32(dest, src->playOK1);
	packet_write_i32(dest, src->playOK2);
}

void packet_server_list_to(struct packet *dest, struct packet_server_list *src)
{
	byte reserved = 0; // Unknown.

	assert(dest);
	assert(src);

	packet_set_type(dest, 0x04);
	packet_write_i8(dest, src->count);
	packet_write_u8(dest, reserved);

	for (i8 i = 0; i < src->count; i += 1) {
		packet_write_u8(dest, src->servers[i].id);
		packet_write_u32(dest, src->servers[i].ip);
		packet_write_u32(dest, src->servers[i].port);
		packet_write_u8(dest, src->servers[i].age_limit);
		packet_write_u8(dest, src->servers[i].pvp);
		packet_write_u16(dest, src->servers[i].players);
		packet_write_u16(dest, src->servers[i].max_players);
		packet_write_u8(dest, src->servers[i].status);
		packet_write_u32(dest, src->servers[i].extra);
		packet_write_u8(dest, src->servers[i].brackets);
	}
}
