#ifndef INCLUDE_LS_SERVER_PACKETS_H
#define INCLUDE_LS_SERVER_PACKETS_H

#include "config.h"
#include "util.h"
#include "packet.h"
#include "server.h"

struct ls_packet_init {
        byte_t session_id[4];
        byte_t protocol[4];
        byte_t modulus[128];
};

enum ls_gg_auth_response {
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
        server_t servers[MAX_SERVERS];
};

void ls_packet_init_pack(packet_t *dest, struct ls_packet_init *src);

void ls_packet_gg_auth_pack(packet_t *dest, struct ls_packet_gg_auth *src);

void ls_packet_ok_pack(packet_t *dest, struct ls_packet_ok *src);

void ls_packet_play_ok_pack(packet_t *dest, struct ls_packet_play_ok *src);

void ls_packet_server_list_pack(packet_t *dest, struct ls_packet_server_list *src);

#endif
