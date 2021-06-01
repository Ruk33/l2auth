#include "include/util.h"
#include "include/packet.h"
#include "include/packet_play_ok.h"

void packet_play_ok_pack(packet_t *dest, packet_play_ok_t *src)
{
        byte_t type = 0x00;

        type = 0x07;

        packet_append_val(dest, type);
        packet_append_val(dest, src->playOK1);
        packet_append_val(dest, src->playOK2);
}
