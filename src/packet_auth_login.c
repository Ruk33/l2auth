#include "include/util.h"
#include "include/packet.h"
#include "include/packet_auth_login.h"

void packet_auth_login_add_character(
        packet_auth_login_t *     dest,
        packet_auth_login_char_t *src)
{
        dest->characters[dest->count] = *src;
        dest->count += 1;
}

void packet_auth_login_pack(packet_t *dest, packet_auth_login_t *src)
{
        byte_t type = 0;

        type = 0x13;

        packet_append_val(dest, type);
        packet_append_val(dest, src->count);

        for (int i = 0; i < src->count; i += 1) {
                // ...
        }
}
