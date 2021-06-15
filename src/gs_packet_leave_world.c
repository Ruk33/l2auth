#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_leave_world.h"

void gs_packet_leave_world_pack(packet_t *dest, gs_packet_leave_world_t *src)
{
        assert(dest);
        assert(src);

        src->type = 0x7e;
        packet_append_val(dest, src->type);
}
