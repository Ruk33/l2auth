#include "include/util.h"
#include "include/packet.h"
#include "include/ls_packet_ok.h"

void ls_packet_ok_pack(packet_t *dest, ls_packet_ok_t *src)
{
        byte_t type = 0;

        byte_t after_key[] = {
                0x00, 0x00, 0x00, 0x00,

                0x00, 0x00, 0x00, 0x00,

                0xea, 0x03, 0x00, 0x00,

                0x00, 0x00, 0x00, 0x00,

                0x00, 0x00, 0x00, 0x00,

                0x02, 0x00, 0x00, 0x00,

                0x00, 0x00, 0x00, 0x00,

                0x00, 0x00, 0x00, 0x00,

                0x60, 0x62, 0xe0, 0x00,

                0x00, 0x00, 0x00,
        };

        type = 0x03;

        packet_append_val(dest, type);
        packet_append_val(dest, src->loginOK1);
        packet_append_val(dest, src->loginOK2);
        packet_append(dest, after_key);
}
