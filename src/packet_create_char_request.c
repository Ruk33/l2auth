#include "include/packet.h"
#include "include/l2_string.h"
#include "include/packet_create_char_request.h"

void packet_create_char_request_unpack(
        packet_create_char_request_t *dest,
        packet_t *src)
{
        packet_t *body = 0;

        size_t max_name_size = 0;

        body = packet_body(src) + 1; // Ignore packet type.

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
