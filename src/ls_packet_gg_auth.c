#include <assert.h>
#include "include/packet.h"
#include "include/ls_packet_gg_auth.h"

void ls_packet_gg_auth_pack(packet_t *dest, ls_packet_gg_auth_t *src)
{
        byte_t type = 0x00;

        assert(dest);
        assert(src);

        type = 0x0b;

        packet_append_val(dest, type);
        packet_append_val(dest, src->response);
}
