#include "include/l2auth.h"
#include "include/login_response.h"

void response_init_encode(struct packet *dest, struct response_init *src)
{
    assert(dest);
    assert(src);

    zero(dest);
    packet_set_type(dest, 0x00);
    packet_write_arr(dest, src->session_id.buf);
    packet_write_arr(dest, src->protocol.buf);
    packet_write_arr(dest, src->modulus.buf);

    // byte pad[16] = {0};
    // packet_write(dest, pad, sizeof(pad));
}

void response_auth_login_ok_encode(struct packet *dest, struct response_auth_login_ok *src)
{
    assert(dest);
    assert(src);

    zero(dest);
    packet_set_type(dest, 0x03);
    packet_write_u32(dest, src->login_ok1);
    packet_write_u32(dest, src->login_ok2);

    byte after_key[] = {
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
    packet_write(dest, after_key, sizeof(after_key));
}

void response_gg_auth_encode(struct packet *dest, struct response_gg_auth *src)
{
    assert(dest);
    assert(src);
    zero(dest);
    packet_set_type(dest, 0x0b);
    packet_write_u32(dest, src->gg_response);
}
