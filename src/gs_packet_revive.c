#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_revive.h"

void gs_packet_revive_pack(packet_t *dest, struct gs_packet_revive *src)
{
        byte_t type = 0x00;

        assert(dest);
        assert(src);
        assert(src->obj_id);

        type = 0x07;

        packet_append_val(dest, type);
        packet_append_val(dest, src->obj_id);
}
