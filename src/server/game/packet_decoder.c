#include <assert.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/packet_reader.h"
#include "include/packet_decoder.h"

void decode_action(struct action *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	struct packet_reader reader = {0};
	packet_read_init(&reader, src);
	packet_read_u32(&dest->target_id, &reader);
	packet_read_i32(&dest->position.x, &reader);
	packet_read_i32(&dest->position.y, &reader);
	packet_read_i32(&dest->position.z, &reader);
	packet_read_u8(&dest->flags, &reader);
}

void decode_attack_request(struct attack_request *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	struct packet_reader reader = {0};
	packet_read_init(&reader, src);
	packet_read_u32(&dest->target_id, &reader);
	packet_read_i32(&dest->position.x, &reader);
	packet_read_i32(&dest->position.y, &reader);
	packet_read_i32(&dest->position.z, &reader);
	packet_read_u8(&dest->flags, &reader);
}

void decode_auth_request(struct auth_request *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	struct packet_reader reader = {0};
	packet_read_init(&reader, src);
	packet_read_str(dest->username.buf, &reader, sizeof(dest->username.buf));
	packet_read_u32(&dest->play_ok2, &reader);
	packet_read_u32(&dest->play_ok1, &reader);
	packet_read_u32(&dest->login_ok1, &reader);
	packet_read_u32(&dest->login_ok2, &reader);
}

void decode_selected_character(struct selected_character *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	struct packet_reader reader = {0};
	packet_read_init(&reader, src);
	packet_read_u32(&dest->index, &reader);
}

void decode_create_character_request(struct create_character_request *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	struct packet_reader reader = {0};
	packet_read_init(&reader, src);
	packet_read_str(dest->name.buf, &reader, sizeof(dest->name.buf));
	packet_read_u32(&dest->race_id, &reader);
	packet_read_u32(&dest->sex, &reader);
	packet_read_u32(&dest->class_id, &reader);
	packet_read_i32(&dest->attrs._int, &reader);
	packet_read_i32(&dest->attrs.str, &reader);
	packet_read_i32(&dest->attrs.con, &reader);
	packet_read_i32(&dest->attrs.men, &reader);
	packet_read_i32(&dest->attrs.dex, &reader);
	packet_read_i32(&dest->attrs.wit, &reader);
	packet_read_u32(&dest->hair_style_id, &reader);
	packet_read_u32(&dest->hair_color_id, &reader);
	packet_read_u32(&dest->face_id, &reader);
}

void decode_move_request(struct move_request *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	struct packet_reader reader = {0};
	packet_read_init(&reader, src);
	packet_read_i32(&dest->position.x, &reader);
	packet_read_i32(&dest->position.y, &reader);
	packet_read_i32(&dest->position.z, &reader);
}

void decode_revive_request(struct revive_request *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	struct packet_reader reader = {0};
	packet_read_init(&reader, src);
	packet_read_u32((u32 *) &dest->option_chosen, &reader);
}

void decode_validate_position_request(struct validate_position_request *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	struct packet_reader reader = {0};
	packet_read_init(&reader, src);
	packet_read_i32(&dest->position.x, &reader);
	packet_read_i32(&dest->position.y, &reader);
	packet_read_i32(&dest->position.z, &reader);
	packet_read_i32(&dest->heading, &reader);
}

void decode_say_request(struct say_request *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	struct packet_reader reader = {0};
	packet_read_init(&reader, src);
	dest->size = packet_read_str(dest->message, &reader, sizeof(dest->message));
}

void decode_bypass(struct bypass *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	struct packet_reader reader = {0};
	packet_read_init(&reader, src);
	dest->size = packet_read_str(dest->command, &reader, sizeof(dest->command));
}

void decode_use_skill_request(struct use_skill_request *dest, struct packet *src)
{
	assert(dest);
	assert(src);

	struct packet_reader reader = {0};
	packet_read_init(&reader, src);
	packet_read_u32(&dest->skill_id, &reader);
	packet_read_u32(&dest->ctrl_pressed, &reader);
	packet_read_u8(&dest->shift_pressed, &reader);
}
