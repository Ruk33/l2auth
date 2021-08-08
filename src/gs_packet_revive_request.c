#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_revive_request.h"

void gs_packet_revive_request_unpack(
        struct gs_packet_revive_request *dest,
        packet_t *src)
{
        packet_t *body = 0;

        assert(dest);
        assert(src);

        body = packet_body(src) + 1; // Ignore packet type.

        byte_read_val(dest->option_chosen, body);
}
