#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_create_char.h"

void gs_packet_create_char_pack(packet_t *dest, gs_packet_create_char_t *src)
{
        byte_t type = 0x00;

        assert(dest);
        assert(src);

        type = 0x19;

        packet_append_val(dest, type);
        packet_append_val(dest, src->response);
}
