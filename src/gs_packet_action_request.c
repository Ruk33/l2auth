#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_action_request.h"

void gs_packet_action_request_unpack(
        gs_packet_action_request_t *dest,
        packet_t *src)
{
        packet_t *body = 0;

        assert(dest);
        assert(src);

        body = packet_body(src) + 1; // Ignore packet type.

        byte_read_val(dest->target_id, body);
        byte_read_val(dest->origin_x, body);
        byte_read_val(dest->origin_y, body);
        byte_read_val(dest->origin_z, body);
        byte_read_val(dest->action, body);
}
