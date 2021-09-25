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
        packet_t *tail = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);
        tail = src + packet_size(src);

        UTIL_READ_BYTES_VAL(dest->target_id, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->origin_x, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->origin_y, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->origin_z, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->action, &body, tail - body);
}

void gs_packet_attack_request_unpack(
        struct gs_packet_attack_request *dest,
        packet_t *src)
{
        packet_t *body = 0;
        packet_t *tail = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);
        tail = src + packet_size(src);

        UTIL_READ_BYTES_VAL(dest->target_id, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->origin_x, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->origin_y, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->origin_z, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->action, &body, tail - body);
}

void gs_packet_auth_request_unpack(
        struct gs_packet_auth_request *dest,
        packet_t *src)
{
        packet_t *body = 0;
        packet_t *tail = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);
        tail = src + packet_size(src);

        l2_string_cpy(
                dest->username,
                (l2_string_t *) body,
                sizeof(dest->username),
                UTIL_MAX(0, tail - body),
                UTIL_MAX(0, tail - body));

        body += l2_string_bytes((l2_string_t *) body, UTIL_MAX(0, tail - body));

        UTIL_READ_BYTES_VAL(dest->playOK2, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->playOK1, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->loginOK1, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->loginOK2, &body, tail - body);
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

        UTIL_READ_BYTES_VAL(dest->index, &body, tail - body);
}

void gs_packet_create_char_request_unpack(
        struct gs_packet_create_char_request *dest,
        packet_t *src)
{
        packet_t *body = 0;
        packet_t *tail = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);
        tail = src + packet_size(src);

        l2_string_cpy(
                dest->name,
                (l2_string_t *) body,
                sizeof(dest->name),
                UTIL_MAX(0, tail - body),
                UTIL_MAX(0, tail - body));

        body += l2_string_bytes((l2_string_t *) body, UTIL_MAX(0, tail - body));

        UTIL_READ_BYTES_VAL(dest->race, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->sex, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->_class, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->_int, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->str, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->con, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->men, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->dex, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->wit, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->hair_style, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->hair_color, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->face, &body, tail - body);
}

void gs_packet_move_request_unpack(
        struct gs_packet_move_request *dest,
        packet_t *src)
{
        packet_t *body = 0;
        packet_t *tail = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);
        tail = src + packet_size(src);

        UTIL_READ_BYTES_VAL(dest->x, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->y, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->z, &body, tail - body);
}

void gs_packet_revive_request_unpack(
        struct gs_packet_revive_request *dest,
        packet_t *src)
{
        packet_t *body = 0;
        packet_t *tail = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);
        tail = src + packet_size(src);

        UTIL_READ_BYTES_VAL(dest->option_chosen, &body, tail - body);
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

        UTIL_READ_BYTES_VAL(dest->x, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->y, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->z, &body, tail - body);
        UTIL_READ_BYTES_VAL(dest->heading, &body, tail - body);
}

void gs_packet_say_request_unpack(
        struct gs_packet_say_request *dest,
        packet_t *src)
{
        packet_t *body = 0;
        packet_t *tail = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);
        tail = src + packet_size(src);

        dest->message = body;
        dest->size = l2_string_bytes(dest->message, UTIL_MAX(0, tail - body));
}

void gs_packet_bypass_request_unpack(
        struct gs_packet_bypass_request *dest,
        packet_t *src)
{
        packet_t *body = 0;
        packet_t *tail = 0;

        assert(dest);
        assert(src);

        body = gs_packet_body_without_type(src);
        tail = src + packet_size(src);

        dest->command = (l2_string_t *) body;
        dest->size = l2_string_bytes(dest->command, UTIL_MAX(0, tail - body));
}
