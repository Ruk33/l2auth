#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_auto_attack.h"

void gs_packet_auto_attack_pack(packet_t *dest, gs_packet_auto_attack_t *src)
{
        byte_t type = 0x00;

        assert(dest);
        assert(src);

        type = 0x2b;
        packet_append_val(dest, type);
        packet_append_val(dest, src->target_id);
}
