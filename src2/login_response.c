#include "include/l2auth.h"

void response_init_encode(struct packet *dest, struct response_init *src)
{
    assert(dest);
    assert(src);

    zero(dest);
    packet_set_type(dest, 0x00);
    packet_write_arr(dest, src->session_id.buf);
    packet_write_arr(dest, src->protocol.buf);
    packet_write_arr(dest, src->modulus.buf);
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

void response_server_list_encode(struct packet *dest, struct server_list *src)
{
    assert(dest);
    assert(src);
    assert(src->count > 0);

    zero(dest);
    packet_set_type(dest, 0x04);
    packet_write_u8(dest, src->count);
    // reserved.
    packet_write_u8(dest, 0);

    for (i8 i = 0; i < src->count; i += 1) {
        packet_write_u8(dest, src->servers[i].id);
        packet_write_u32(dest, src->servers[i].ip);
        packet_write_u32(dest, src->servers[i].port);
        packet_write_u8(dest, src->servers[i].age_limit);
        packet_write_u8(dest, src->servers[i].pvp);
        packet_write_u16(dest, src->servers[i].players);
        packet_write_u16(dest, src->servers[i].max_players);
        packet_write_u8(dest, src->servers[i].status);
        packet_write_u32(dest, src->servers[i].extra);
        packet_write_u8(dest, src->servers[i].brackets);
    }
}

void response_play_ok_encode(struct packet *dest, struct response_play_ok *src)
{
    assert(dest);
    assert(src);
    zero(dest);
    packet_set_type(dest, 0x07);
    packet_write_u32(dest, src->play_ok1);
    packet_write_u32(dest, src->play_ok2);
}
