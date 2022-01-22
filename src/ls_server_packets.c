#include <assert.h>

struct ls_packet_init {
    byte_t session_id[4];
    byte_t protocol[4];
    byte_t modulus[128];
};

enum ls_gg_auth_response
{
    PACKET_GG_AUTH_RESPONSE_SKIP = 0x0b,
};

struct ls_packet_gg_auth {
    enum ls_gg_auth_response response;
};

struct ls_packet_ok {
    i32_t loginOK1;
    i32_t loginOK2;
    byte_t after_key[39]; // Unknown
};

struct ls_packet_play_ok {
    i32_t playOK1;
    i32_t playOK2;
};

struct ls_packet_server_list {
    u8_t count;
    byte_t reserved; // Unknown
    struct ls_server servers[MAX_SERVERS];
};

void ls_packet_init_pack(packet_t *dest, struct ls_packet_init *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    macro_packet_append_val(dest, type);
    macro_packet_append(dest, src->session_id);
    macro_packet_append(dest, src->protocol);
    macro_packet_append(dest, src->modulus);
}

void ls_packet_gg_auth_pack(packet_t *dest, struct ls_packet_gg_auth *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x0b;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->response);
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

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->loginOK1);
    macro_packet_append_val(dest, src->loginOK2);
    macro_packet_append(dest, after_key);
}

void ls_packet_play_ok_pack(packet_t *dest, struct ls_packet_play_ok *src)
{
    byte_t type = 0x00;

    assert(dest);
    assert(src);

    type = 0x07;

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->playOK1);
    macro_packet_append_val(dest, src->playOK2);
}

void ls_packet_server_list_pack(packet_t *dest,
                                struct ls_packet_server_list *src)
{
    byte_t type = 0;

    type = 0x04;

    assert(dest);
    assert(src);

    macro_packet_append_val(dest, type);
    macro_packet_append_val(dest, src->count);
    macro_packet_append_val(dest, src->reserved);

    for (u8_t i = 0; i < src->count; i += 1) {
        macro_packet_append_val(dest, src->servers[i].id);
        macro_packet_append_val(dest, src->servers[i].ip);
        macro_packet_append_val(dest, src->servers[i].port);
        macro_packet_append_val(dest, src->servers[i].age_limit);
        macro_packet_append_val(dest, src->servers[i].pvp);
        macro_packet_append_val(dest, src->servers[i].players);
        macro_packet_append_val(dest, src->servers[i].max_players);
        macro_packet_append_val(dest, src->servers[i].status);
        macro_packet_append_val(dest, src->servers[i].extra);
        macro_packet_append_val(dest, src->servers[i].brackets);
    }
}
