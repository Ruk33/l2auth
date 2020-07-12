#ifndef L2AUTH_GAME_SERVER_H
#define L2AUTH_GAME_SERVER_H

#include <stdlib.h>
#include <core/l2_packet.h>
#include <game/connection.h>
#include <game/client.h>

struct GameServer
{
        os_socket_handler* socket_handler;
        size_t accepted_clients;
        struct GameConnection** clients;
};

typedef void *(* game_server_request_handler)(void*);

void game_server_broadcast_packet_to_clients
(
        struct GameServer* server,
        l2_packet* packet
);

void game_server_broadcast_packet
(
        struct GameServer* server,
        struct GameClient* from,
        l2_packet* packet
);

void game_server_free(struct GameServer* server);

void game_server_start_or_die(unsigned short port, size_t max_players);

#endif
