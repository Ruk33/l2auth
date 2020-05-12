#ifndef L2AUTH_GAME_CLIENT_H
#define L2AUTH_GAME_CLIENT_H

#include <stdlib.h>
#include <core/byte_builder.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/session_key.h>
#include <os/socket.h>
#include <game/entity/character.h>

struct GameClient;

size_t game_client_struct_size();

struct L2SessionKey* game_client_session(struct GameClient* client);

struct GameEntityCharacter* game_client_get_char(struct GameClient* client);

void game_client_init(struct GameClient* client);

byte_builder* game_client_byte_builder
(
        struct GameClient* client,
        size_t how_much
);

void* game_client_alloc(struct GameClient* client, size_t how_much);

void game_client_alloc_free(struct GameClient* client, void* block);

void* game_client_alloc_temp_mem(struct GameClient* client, size_t how_much);

l2_raw_packet* game_client_create_raw_packet
(
        struct GameClient* client,
        unsigned char* content,
        unsigned short content_size
);

l2_packet* game_client_create_packet
(
        struct GameClient* client,
        l2_packet_type type,
        unsigned char* content,
        unsigned short content_size
);

void game_client_accept
(
        struct GameClient* client,
        os_socket_handler* server_socket
);

void game_client_close(struct GameClient* client);

void game_client_send_packet(struct GameClient* client, l2_raw_packet* packet);

l2_raw_packet* game_client_wait_packet(struct GameClient* client);

int game_client_connection_ended(struct GameClient* client);

#endif
