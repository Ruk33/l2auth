#include "include/game_server.h"

void request_auth_decode(struct request_auth *dest, struct packet *src)
{
    assert(dest);
    assert(src);
    packet_read_arr(dest->username.buf, src);
    packet_read_u32(&dest->play_ok2, src);
    packet_read_u32(&dest->play_ok1, src);
    packet_read_u32(&dest->login_ok1, src);
    packet_read_u32(&dest->login_ok2, src);
}
