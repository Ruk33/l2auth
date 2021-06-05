#include "include/util.h"
#include "include/packet.h"
#include "include/ls_packet_play_ok.h"

void ls_packet_play_ok_pack(packet_t *dest, ls_packet_play_ok_t *src)
{
        byte_t type = 0x00;

        type = 0x07;

        packet_append_val(dest, type);
        packet_append_val(dest, src->playOK1);
        packet_append_val(dest, src->playOK2);
}
