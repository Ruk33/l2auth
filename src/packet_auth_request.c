#include "include/util.h"
#include "include/l2_string.h"
#include "include/packet.h"
#include "include/packet_auth_request.h"

void packet_auth_request_unpack(packet_auth_request_t *dest, packet_t *src)
{
        byte_t *body = 0;

        size_t max_username_size = 0;

        body = packet_body(src) + 1; // Remove packet type.

        max_username_size = sizeof(dest->username);
        l2_string_cpy(dest->username, (l2_string_t *) body, max_username_size);

        body += l2_string_bytes((l2_string_t *) body);

        byte_read_val(dest->playOK2, body);
        byte_read_val(dest->playOK1, body);
        byte_read_val(dest->loginOK1, body);
        byte_read_val(dest->loginOK2, body);
}
