#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_quest_list.h"

void gs_packet_quest_list_pack(packet_t *dest, gs_packet_quest_list_t *src)
{
        byte_t type = 0x00;

        type = 0x80;

        packet_append_val(dest, type);
        packet_append_n(dest, src->empty, sizeof(src->empty));
}
