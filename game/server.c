#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <game/request/hot_code_reload_entry_point/handler.h>
#include <game/connection.h>
#include <game/client.h>
#include <game/service/crypt/packet/encrypt.h>
#include <game/request/protocol_version/handler.h>
#include "server.h"

struct GameServer
{
        os_socket_handler* socket_handler;
        size_t accepted_clients;
        struct GameConnection** clients;
};

void game_server_broadcast_packet_to_clients
(
        struct GameServer* server,
        l2_packet* packet
)
{
        assert(server);
        assert(packet);

        struct GameConnection* conn;
        struct GameClient* client;
        l2_raw_packet* encrypted_packet;
        size_t packet_size = l2_raw_packet_get_size(packet);
        size_t full_packet_size = l2_raw_packet_calculate_size(packet_size);

        log_info("Broadcasting packet to all clients");
        log_info("Clients count: %ld", server->accepted_clients);

        for (size_t i = 0; i < server->accepted_clients; i++) {
                conn = server->clients[i];
                client = conn->client;

                encrypted_packet = game_client_alloc_temp_mem(
                        client,
                        full_packet_size
                );

                memcpy(encrypted_packet, packet, full_packet_size);

                game_client_send_packet(
                        client,
                        game_service_crypt_packet_encrypt(encrypted_packet, conn->encrypt_key)
                );
        }
}

void game_server_broadcast_packet
(
        struct GameServer* server,
        struct GameClient* from,
        l2_packet* packet
)
{
        assert(server);
        assert(from);
        assert(packet);

        if (server->accepted_clients - 1 == 0) return;

        struct GameConnection* conn;
        struct GameClient* client;
        l2_raw_packet* encrypted_packet;
        size_t packet_size = l2_raw_packet_get_size(packet);
        size_t full_packet_size = l2_raw_packet_calculate_size(packet_size);

        log_info("Broadcasting packet to the rest of the clients");
        log_info("Clients count (ignoring myself): %ld", server->accepted_clients - 1);

        for (size_t i = 0; i < server->accepted_clients; i++) {
                conn = server->clients[i];
                client = conn->client;

                if (client == from) continue;

                encrypted_packet = game_client_alloc_temp_mem(
                        client,
                        full_packet_size
                );

                memcpy(encrypted_packet, packet, full_packet_size);

                game_client_send_packet(
                        client,
                        game_service_crypt_packet_encrypt(encrypted_packet, conn->encrypt_key)
                );
        }
}

void game_server_free(struct GameServer* server)
{
        assert(server);
        if (server->clients) free(server->clients);
        if (server->socket_handler) free(server->socket_handler);
        free(server);
}

struct GameServer* game_server_create(size_t max_players)
{
        struct GameServer* server = calloc(1, sizeof(struct GameServer));

        if (!server) return NULL;

        server->clients = malloc(sizeof(struct GameConnection *) * max_players);
        server->socket_handler = calloc(1, os_socket_handler_size());

        if (!server->clients || !server->socket_handler) {
                game_server_free(server);
                return NULL;
        }

        server->accepted_clients = 0;

        return server;
}

void game_server_listen(struct GameServer* server, unsigned short port)
{
        assert(server);

        os_socket_connect(server->socket_handler);
        os_socket_bind(server->socket_handler, port);
        os_socket_listen(server->socket_handler, 3);
}

struct GameConnection* game_server_get_client
(
        struct GameServer* server,
        size_t index
)
{
        assert(server);
        struct GameConnection* conn = calloc(1, sizeof(struct GameConnection));
        conn->client = calloc(1, game_client_struct_size());
        conn->handler = game_request_protocol_version_handler;
        server->clients[index] = conn;
        return conn;
}

void game_server_accept_client
(
        struct GameServer* server,
        game_server_request_handler* handler
)
{
        assert(server);
        assert(handler);

        struct GameConnection* conn = game_server_get_client(
                server,
                server->accepted_clients
        );

        conn->server = server;

        game_client_init(conn->client);
        game_client_accept(conn->client, server->socket_handler);
        server->accepted_clients += 1;
        pthread_create(&conn->thread, NULL, handler, conn);
        //server->accepted_clients -= 1;
        //l2_client_close(client);
}

void game_server_start
(
        struct GameServer* server,
        unsigned short port,
        game_server_request_handler* handler
)
{
        assert(server);
        assert(handler);
        game_server_listen(server, port);
        while(1) game_server_accept_client(server, handler);
}

void game_server_start_or_die(unsigned short port, size_t max_players)
{
        struct GameServer* server = game_server_create(max_players);

        log_info("Starting gameserver");

        if (server == NULL) {
                log_fatal("Not able to allocate memory for gameserver");
                exit(1);
        }

        game_server_start(server, port, game_request_hot_code_reload_entry_point_handler);
}
