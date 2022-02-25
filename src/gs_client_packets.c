#include "include/util.h"
#include "include/gs_client_packets.h"

// Todo: IMPORTANT, we need to check if packets are correct.
// right now, we are assuming all packets are valid.

// Not sure about this one, we may have requests preallocating space...
// Maybe add a third parameter to byte_read_val that holds
// what's left from the buffer. If it's not enough to read it, then return 0?

// define gs_packet_ignore_if_invalid(request, to_unpack)
//         if (packet_size(request) < sizeof(to_unpack)) {
//                 return 0;
//         }

static packet_t *gs_packet_body_without_type(packet_t *src)
{
    assert(src);
    return packet_body(src) + 1; // Ignore packet type.
}

void gs_packet_action_request_unpack(struct gs_packet_action_request *dest,
                                     packet_t *src)
{
    packet_t *body = 0;
    packet_t *tail = 0;

    assert(dest);
    assert(src);

    body = gs_packet_body_without_type(src);
    tail = src + packet_size(src);

    macro_util_read_bytes_val(dest->target_id, &body, tail - body);
    macro_util_read_bytes_val(dest->origin_x, &body, tail - body);
    macro_util_read_bytes_val(dest->origin_y, &body, tail - body);
    macro_util_read_bytes_val(dest->origin_z, &body, tail - body);
    macro_util_read_bytes_val(dest->action, &body, tail - body);
}

void gs_packet_attack_request_unpack(struct gs_packet_attack_request *dest,
                                     packet_t *src)
{
    packet_t *body = 0;
    packet_t *tail = 0;

    assert(dest);
    assert(src);

    body = gs_packet_body_without_type(src);
    tail = src + packet_size(src);

    macro_util_read_bytes_val(dest->target_id, &body, tail - body);
    macro_util_read_bytes_val(dest->origin_x, &body, tail - body);
    macro_util_read_bytes_val(dest->origin_y, &body, tail - body);
    macro_util_read_bytes_val(dest->origin_z, &body, tail - body);
    macro_util_read_bytes_val(dest->action, &body, tail - body);
}

void gs_packet_auth_request_unpack(struct gs_packet_auth_request *dest,
                                   packet_t *src)
{
    packet_t *body = 0;
    packet_t *tail = 0;

    struct buffer body_buffer = { 0 };

    assert(dest);
    assert(src);

    body = gs_packet_body_without_type(src);
    tail = src + packet_size(src);

    body_buffer.buf = body;
    body_buffer.size = macro_util_max(0, tail - body);

    l2_string_cpy(&util_macro_buf_from_arr(dest->username), &body_buffer, body_buffer.size);

    body += l2_string_bytes_from_buf(&body_buffer);

    macro_util_read_bytes_val(dest->playOK2, &body, tail - body);
    macro_util_read_bytes_val(dest->playOK1, &body, tail - body);
    macro_util_read_bytes_val(dest->loginOK1, &body, tail - body);
    macro_util_read_bytes_val(dest->loginOK2, &body, tail - body);
}

void gs_packet_char_select_request_unpack(
    struct gs_packet_char_select_request *dest,
    packet_t *src)
{
    packet_t *body = 0;
    packet_t *tail = 0;

    assert(dest);
    assert(src);

    body = gs_packet_body_without_type(src);
    tail = src + packet_size(src);

    macro_util_read_bytes_val(dest->index, &body, tail - body);
}

void gs_packet_create_char_request_unpack(
    struct gs_packet_create_char_request *dest,
    packet_t *src)
{
    packet_t *body = 0;
    packet_t *tail = 0;

    struct buffer body_buffer = { 0 };

    assert(dest);
    assert(src);

    body = gs_packet_body_without_type(src);
    tail = src + packet_size(src);

    body_buffer.buf = body;
    body_buffer.size = macro_util_max(0, tail - body);

    l2_string_cpy(&util_macro_buf_from_arr(dest->name), &body_buffer, body_buffer.size);

    body += l2_string_bytes_from_buf(&body_buffer);

    macro_util_read_bytes_val(dest->race, &body, tail - body);
    macro_util_read_bytes_val(dest->sex, &body, tail - body);
    macro_util_read_bytes_val(dest->_class, &body, tail - body);
    macro_util_read_bytes_val(dest->_int, &body, tail - body);
    macro_util_read_bytes_val(dest->str, &body, tail - body);
    macro_util_read_bytes_val(dest->con, &body, tail - body);
    macro_util_read_bytes_val(dest->men, &body, tail - body);
    macro_util_read_bytes_val(dest->dex, &body, tail - body);
    macro_util_read_bytes_val(dest->wit, &body, tail - body);
    macro_util_read_bytes_val(dest->hair_style, &body, tail - body);
    macro_util_read_bytes_val(dest->hair_color, &body, tail - body);
    macro_util_read_bytes_val(dest->face, &body, tail - body);
}

void gs_packet_move_request_unpack(struct gs_packet_move_request *dest,
                                   packet_t *src)
{
    packet_t *body = 0;
    packet_t *tail = 0;

    assert(dest);
    assert(src);

    body = gs_packet_body_without_type(src);
    tail = src + packet_size(src);

    macro_util_read_bytes_val(dest->x, &body, tail - body);
    macro_util_read_bytes_val(dest->y, &body, tail - body);
    macro_util_read_bytes_val(dest->z, &body, tail - body);
}

void gs_packet_revive_request_unpack(struct gs_packet_revive_request *dest,
                                     packet_t *src)
{
    packet_t *body = 0;
    packet_t *tail = 0;

    assert(dest);
    assert(src);

    body = gs_packet_body_without_type(src);
    tail = src + packet_size(src);

    macro_util_read_bytes_val(dest->option_chosen, &body, tail - body);
}

void gs_packet_validate_pos_request_unpack(
    struct gs_packet_validate_pos_request *dest,
    packet_t *src)
{
    packet_t *body = 0;
    packet_t *tail = 0;

    assert(dest);
    assert(src);

    body = gs_packet_body_without_type(src);
    tail = src + packet_size(src);

    macro_util_read_bytes_val(dest->x, &body, tail - body);
    macro_util_read_bytes_val(dest->y, &body, tail - body);
    macro_util_read_bytes_val(dest->z, &body, tail - body);
    macro_util_read_bytes_val(dest->heading, &body, tail - body);
}

void gs_packet_say_request_unpack(struct gs_packet_say_request *dest,
                                  packet_t *src)
{
    packet_t *body = 0;
    packet_t *tail = 0;

    struct buffer body_buffer = { 0 };

    assert(dest);
    assert(src);

    body = gs_packet_body_without_type(src);
    tail = src + packet_size(src);

    body_buffer.buf = body;
    body_buffer.size = macro_util_max(0, tail - body);

    dest->message = body;
    dest->size = l2_string_bytes_from_buf(&body_buffer);
}

void gs_packet_bypass_request_unpack(struct gs_packet_bypass_request *dest,
                                     packet_t *src)
{
    packet_t *body = 0;
    packet_t *tail = 0;

    assert(dest);
    assert(src);

    body = gs_packet_body_without_type(src);
    tail = src + packet_size(src);

    dest->command = (l2_string_t *) body;
    dest->size = l2_string_bytes(dest->command, macro_util_max(0, tail - body));
}

void gs_packet_use_skill_request_unpack(struct gs_packet_skill_use_request *dest,
                                        packet_t *src)
{
    packet_t *body = 0;
    packet_t *tail = 0;

    assert(dest);
    assert(src);

    body = gs_packet_body_without_type(src);
    tail = src + packet_size(src);

    macro_util_read_bytes_val(dest->skill_id, &body, tail - body);
    macro_util_read_bytes_val(dest->ctrl_pressed, &body, tail - body);
    macro_util_read_bytes_val(dest->shift_pressed, &body, tail - body);
}
