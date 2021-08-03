#include <assert.h>
#include "include/gs_packet_restart.h"

void gs_packet_restart_pack(packet_t *dest, gs_packet_restart_t *src)
{
        byte_t type = 0x00;

        assert(dest);
        assert(src);

        type = 0x5f;

        packet_append_val(dest, type);
        packet_append_val(dest, src->response);
}
