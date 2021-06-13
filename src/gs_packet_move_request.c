#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_move_request.h"

void gs_packet_move_request_unpack(gs_packet_move_request_t *dest, packet_t *s)
{
        packet_t *body = 0;

        assert(dest);
        assert(s);

        body = packet_body(s) + 1; // Ignore packet type.

        byte_read_val(dest->x, body);
        byte_read_val(dest->y, body);
        byte_read_val(dest->z, body);
}
