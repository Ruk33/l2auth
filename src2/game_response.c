#include "include/game_server.h"

void response_protocol_encode(struct packet *dest, struct response_protocol *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x00);
    packet_write_arr(dest, src->protocol.buf);
    packet_checksum(dest);
}

void response_auth_login_encode(struct packet *dest, struct response_auth_login *src)
{
    assert(dest);
    assert(src);
    packet_set_type(dest, 0x13);
    packet_write_u32(dest, src->count);
    // todo: send the rest of the packet, which means, character list!
}
