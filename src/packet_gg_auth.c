#include "include/packet.h"
#include "include/packet_gg_auth.h"

void packet_gg_auth_pack(packet_t *dest, packet_gg_auth_t *src)
{
        byte_t type = 0;

        type = 0x0b;
        packet_append_val(dest, type);
        packet_append_val(dest, src->response);
}
