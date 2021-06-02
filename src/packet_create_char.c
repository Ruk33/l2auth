#include "include/util.h"
#include "include/packet.h"
#include "include/packet_create_char.h"

void packet_create_char_pack(packet_t *dest, packet_create_char_t *src)
{
        byte_t type = 0x00;

        type = 0x19;

        packet_append_val(dest, type);
        packet_append_val(dest, src->response);
}
