#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_validate_pos_request.h"

void gs_packet_validate_pos_request_unpack(
        gs_packet_validate_pos_request_t *dest,
        packet_t *src)
{
        packet_t *body = 0;

        assert(dest);
        assert(src);

        body = packet_body(src) + 1; // Ignore packet type.

        byte_read_val(dest->x, body);
        byte_read_val(dest->y, body);
        byte_read_val(dest->z, body);
        byte_read_val(dest->heading, body);
}
