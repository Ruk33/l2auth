#include <assert.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/packet_read.h"
#include "include/client_packet.h"

void packet_action_request_from(struct packet_action_request *dest, struct packet *src)
{
    struct packet_read reader = { 0 };

    assert(dest);
    assert(src);

    packet_read_init(&reader, src);
    packet_read_u32(&dest->target_id, &reader);
    packet_read_i32(&dest->origin_pos.x, &reader);
    packet_read_i32(&dest->origin_pos.y, &reader);
    packet_read_i32(&dest->origin_pos.z, &reader);
    packet_read_u8(&dest->action, &reader);
}

void packet_attack_request_from(struct packet_attack_request *dest, struct packet *src)
{
    struct packet_read reader = { 0 };

    assert(dest);
    assert(src);

    packet_read_init(&reader, src);
    packet_read_u32(&dest->target_id, &reader);
    packet_read_i32(&dest->origin_pos.x, &reader);
    packet_read_i32(&dest->origin_pos.y, &reader);
    packet_read_i32(&dest->origin_pos.z, &reader);
    packet_read_u8(&dest->action, &reader);
}

void packet_auth_request_from(struct packet_auth_request *dest, struct packet *src)
{
    struct packet_read reader = { 0 };

    assert(dest);
    assert(src);

    packet_read_init(&reader, src);
    packet_read_str(dest->username.buf, &reader, sizeof(dest->username.buf));
    packet_read_i32(&dest->playOK2, &reader);
    packet_read_i32(&dest->playOK1, &reader);
    packet_read_i32(&dest->loginOK1, &reader);
    packet_read_i32(&dest->loginOK2, &reader);
}

void packet_char_select_request_from(struct packet_char_select_request *dest, struct packet *src)
{
    struct packet_read reader = { 0 };

    assert(dest);
    assert(src);

    packet_read_init(&reader, src);
    packet_read_u32(&dest->index, &reader);
}

void packet_create_char_request_from(struct packet_create_char_request *dest, struct packet *src)
{
    struct packet_read reader = { 0 };

    assert(dest);
    assert(src);

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
    packet_read_u32(&dest->hair_style, &reader);
    packet_read_u32(&dest->hair_color, &reader);
    packet_read_u32(&dest->face, &reader);
}

void packet_move_request_from(struct packet_move_request *dest, struct packet *src)
{
    struct packet_read reader = { 0 };

    assert(dest);
    assert(src);

    packet_read_init(&reader, src);
    packet_read_i32(&dest->position.x, &reader);
    packet_read_i32(&dest->position.y, &reader);
    packet_read_i32(&dest->position.z, &reader);
}

void packet_revive_request_from(struct packet_revive_request *dest, struct packet *src)
{
    struct packet_read reader = { 0 };

    assert(dest);
    assert(src);

    packet_read_init(&reader, src);
    packet_read_i32(&dest->option_chosen, &reader);
}

void packet_validate_pos_request_from(struct packet_validate_pos_request *dest, struct packet *src)
{
    struct packet_read reader = { 0 };

    assert(dest);
    assert(src);

    packet_read_init(&reader, src);
    packet_read_i32(&dest->position.x, &reader);
    packet_read_i32(&dest->position.y, &reader);
    packet_read_i32(&dest->position.z, &reader);
    packet_read_i32(&dest->position.heading, &reader);
}

void packet_say_request_from(struct packet_say_request *dest, struct packet *src)
{
    struct packet_read reader = { 0 };

    assert(dest);
    assert(src);

    packet_read_init(&reader, src);
    dest->size = packet_read_str(dest->message, &reader, sizeof(dest->message));
}

void packet_bypass_request_from(struct packet_bypass_request *dest, struct packet *src)
{
    struct packet_read reader = { 0 };

    assert(dest);
    assert(src);

    packet_read_init(&reader, src);
    dest->size = packet_read_str(dest->command, &reader, sizeof(dest->command));
}

void packet_skill_use_request_from(struct packet_skill_use_request *dest, struct packet *src)
{
    struct packet_read reader = { 0 };

    assert(dest);
    assert(src);

    packet_read_init(&reader, src);
    packet_read_u32(&dest->skill_id, &reader);
    packet_read_u32(&dest->ctrl_pressed, &reader);
    packet_read_u8(&dest->shift_pressed, &reader);
}
