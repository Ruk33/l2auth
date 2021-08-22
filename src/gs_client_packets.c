#include "include/util.h"
#include "include/packet.h"
#include "include/l2_string.h"
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

void gs_packet_action_request_unpack(
        struct gs_packet_action_request *dest,
        packet_t *src)
{
        packet_t *body = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);

        byte_read_val(dest->target_id, body);
        byte_read_val(dest->origin_x, body);
        byte_read_val(dest->origin_y, body);
        byte_read_val(dest->origin_z, body);
        byte_read_val(dest->action, body);
}

void gs_packet_auth_request_unpack(
        struct gs_packet_auth_request *dest,
        packet_t *src)
{
        byte_t *body = 0;

        size_t max_username_size = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);

        max_username_size = sizeof(dest->username);

        l2_string_cpy(dest->username, (l2_string_t *) body, max_username_size);

        body += l2_string_bytes((l2_string_t *) body);

        byte_read_val(dest->playOK2, body);
        byte_read_val(dest->playOK1, body);
        byte_read_val(dest->loginOK1, body);
        byte_read_val(dest->loginOK2, body);
}

void gs_packet_char_select_request_unpack(
        struct gs_packet_char_select_request *dest,
        packet_t *src)
{
        packet_t *body = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);
        byte_read_val(dest->index, body);
}

void gs_packet_create_char_request_unpack(
        struct gs_packet_create_char_request *dest,
        packet_t *src)
{
        packet_t *body = 0;

        size_t max_name_size = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);

        max_name_size = sizeof(dest->name);

        l2_string_cpy(dest->name, (l2_string_t *) body, max_name_size);

        body += l2_string_bytes((l2_string_t *) body);

        byte_read_val(dest->race, body);
        byte_read_val(dest->sex, body);
        byte_read_val(dest->_class, body);
        byte_read_val(dest->_int, body);
        byte_read_val(dest->str, body);
        byte_read_val(dest->con, body);
        byte_read_val(dest->men, body);
        byte_read_val(dest->dex, body);
        byte_read_val(dest->wit, body);
        byte_read_val(dest->hair_style, body);
        byte_read_val(dest->hair_color, body);
        byte_read_val(dest->face, body);
}

void gs_packet_move_request_unpack(
        struct gs_packet_move_request *dest,
        packet_t *src)
{
        packet_t *body = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);

        byte_read_val(dest->x, body);
        byte_read_val(dest->y, body);
        byte_read_val(dest->z, body);
}

void gs_packet_revive_request_unpack(
        struct gs_packet_revive_request *dest,
        packet_t *src)
{
        packet_t *body = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);

        byte_read_val(dest->option_chosen, body);
}

void gs_packet_validate_pos_request_unpack(
        struct gs_packet_validate_pos_request *dest,
        packet_t *src)
{
        packet_t *body = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);

        byte_read_val(dest->x, body);
        byte_read_val(dest->y, body);
        byte_read_val(dest->z, body);
        byte_read_val(dest->heading, body);
}

void gs_packet_say_request_unpack(
        struct gs_packet_say_request *dest,
        packet_t *src)
{
        byte_t *body = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);

        dest->message = body;
        dest->len     = l2_string_len(dest->message);
}
