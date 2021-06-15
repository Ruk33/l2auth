#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_target_selected.h"

void gs_packet_target_selected_pack(
        packet_t *dest,
        gs_packet_target_selected_t *src)
{
        byte_t type = 0x00;

        assert(dest);
        assert(src);

        type = 0xa6;

        packet_append_val(dest, type);
        packet_append_val(dest, src->target_id);
        packet_append_val(dest, src->color);
}
