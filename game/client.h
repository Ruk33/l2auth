#ifndef L2AUTH_GAME_CLIENT_H
#define L2AUTH_GAME_CLIENT_H

#include <stdlib.h>
#include <core/memory.h>
#include <core/circular_memory_alloc.h>
#include <core/byte_builder.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/session_key.h>
#include <os/socket.h>
#include <game/entity/character.h>

#define MAX_DATA_FROM_PACKET 65536
#define TEMP_MEMORY_PER_CLIENT_IN_BYTES 131072
#define MEMORY_PER_CLIENT_IN_BYTES 131072

struct GameClient
{
        os_socket_handler* socket_handler;
        struct L2SessionKey session;

        size_t received_data_size;
        unsigned char received_data[MAX_DATA_FROM_PACKET];

        circular_memory_space temp_memory[TEMP_MEMORY_PER_CLIENT_IN_BYTES];
        memory preallocated_memory[MEMORY_PER_CLIENT_IN_BYTES];

        struct GameEntityCharacter character;
};

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
