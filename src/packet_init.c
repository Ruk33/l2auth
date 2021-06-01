#include "include/util.h"
#include "include/packet.h"
#include "include/packet_init.h"

void packet_init(packet_init_t *dest, byte_t *modulus)
{
        byte_t session_id[] = { 0xfd, 0x8a, 0x22, 0x00 };
        byte_t protocol[]   = { 0x5a, 0x78, 0x00, 0x00 };

        bytes_cpy(dest->session_id, session_id, sizeof(dest->session_id));
        bytes_cpy(dest->protocol, protocol, sizeof(dest->protocol));
        bytes_cpy(dest->modulus, modulus, sizeof(dest->modulus));
}

void packet_init_pack(packet_t *dest, packet_init_t *src)
{
        byte_t type = 0x00;

        packet_append_val(dest, type);
        packet_append(dest, src->session_id);
        packet_append(dest, src->protocol);
        packet_append(dest, src->modulus);
}
