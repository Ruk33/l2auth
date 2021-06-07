#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_d0.h"

void gs_packet_d0_pack(packet_t *dest, gs_packet_d0_t *src)
{
        byte_t type = 0x00;

        assert(dest);
        assert(src);

        type = 0xFE;

        src->unknown = 0x1B;

        packet_append_val(dest, type);
        packet_append_val(dest, src->unknown);
        packet_append_val(dest, src->manor_size);
}
