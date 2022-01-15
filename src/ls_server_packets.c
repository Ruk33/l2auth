#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/ls_server_packets.h"

void ls_packet_init_pack(packet_t *dest, struct ls_packet_init *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    packet_append_val(dest, type);
    packet_append(dest, src->session_id);
    packet_append(dest, src->protocol);
    packet_append(dest, src->modulus);
}

void ls_packet_gg_auth_pack(packet_t *dest, struct ls_packet_gg_auth *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x0b;

    packet_append_val(dest, type);
    packet_append_val(dest, src->response);
}

void ls_packet_ok_pack(packet_t *dest, struct ls_packet_ok *src)
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

    assert(dest);
    assert(src);

    type = 0x03;

    packet_append_val(dest, type);
    packet_append_val(dest, src->loginOK1);
    packet_append_val(dest, src->loginOK2);
    packet_append(dest, after_key);
}

void ls_packet_play_ok_pack(packet_t *dest, struct ls_packet_play_ok *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x07;

    packet_append_val(dest, type);
    packet_append_val(dest, src->playOK1);
    packet_append_val(dest, src->playOK2);
}

void ls_packet_server_list_pack(packet_t *dest,
                                struct ls_packet_server_list *src)
{
    byte_t type = 0;

    type = 0x04;

    assert(dest);
    assert(src);

    packet_append_val(dest, type);
    packet_append_val(dest, src->count);
    packet_append_val(dest, src->reserved);

    for (u8_t i = 0; i < src->count; i += 1) {
        packet_append_val(dest, src->servers[i].id);
        packet_append_val(dest, src->servers[i].ip);
        packet_append_val(dest, src->servers[i].port);
        packet_append_val(dest, src->servers[i].age_limit);
        packet_append_val(dest, src->servers[i].pvp);
        packet_append_val(dest, src->servers[i].players);
        packet_append_val(dest, src->servers[i].max_players);
        packet_append_val(dest, src->servers[i].status);
        packet_append_val(dest, src->servers[i].extra);
        packet_append_val(dest, src->servers[i].brackets);
    }
}
