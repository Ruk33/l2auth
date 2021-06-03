#include "include/util.h"
#include "include/packet.h"
#include "include/packet_d0.h"

void packet_d0_pack(packet_t *dest, packet_d0_t *src)
{
        byte_t type = 0x00;

        type = 0xFE;

        src->unknown = 0x1B;

        packet_append_val(dest, type);
        packet_append_val(dest, src->unknown);
        packet_append_val(dest, src->manor_size);
}
