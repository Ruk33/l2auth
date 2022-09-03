#include <assert.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/packet_encoder.h"

void encode_init(struct packet *dest, struct init *src)
{
	assert(dest);
	assert(src);

	*dest = (struct packet) {0};
	packet_set_type(dest, 0x00);
	packet_write(dest, src->session_id.buf, sizeof(src->session_id.buf));
	packet_write(dest, src->protocol.buf, sizeof(src->protocol.buf));
	packet_write(dest, src->modulus.buf, sizeof(src->modulus.buf));

	byte pad[16] = {0};
	packet_write(dest, pad, sizeof(pad));
}

void encode_gg_auth(struct packet *dest, struct gg_auth *src)
{
	assert(dest);
	assert(src);
	*dest = (struct packet) {0};
	packet_set_type(dest, 0x0b);
	packet_write_i32(dest, src->gg_response);
}

void encode_ok(struct packet *dest, struct ok *src)
{
	assert(dest);
	assert(src);

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
	*dest = (struct packet) {0};
	packet_set_type(dest, 0x03);
	packet_write_u32(dest, src->login_ok1);
	packet_write_u32(dest, src->login_ok2);
	packet_write(dest, after_key, sizeof(after_key));
}

void encode_play_ok(struct packet *dest, struct play_ok *src)
{
	assert(dest);
	assert(src);
	*dest = (struct packet) {0};
	packet_set_type(dest, 0x07);
	packet_write_u32(dest, src->play_ok1);
	packet_write_u32(dest, src->play_ok2);
}

void encode_server_list(struct packet *dest, struct server_list *src)
{
	assert(dest);
	assert(src);

	*dest = (struct packet) {0};
	packet_set_type(dest, 0x04);
	packet_write_u8(dest, src->count);
	packet_write_u8(dest, 0); // Reserved.

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
