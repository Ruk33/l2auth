#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <log/log.h>
#include <core/connection_thread.h>
#include <core/l2_client.h>
#include <core/l2_server.h>
#include <os/socket.h>
#include <game/service/crypt/packet/encrypt.h>

struct L2Server {
        os_socket_handler* socket_handler;
        size_t accepted_clients;
        struct ConnectionThread** clients;
};

void l2_server_broadcast_packet_to_clients
(
        struct L2Server* server,
        l2_packet* packet
)
{
        assert(server);
        assert(packet);

        struct ConnectionThread* conn;
        struct L2Client* client;
        l2_raw_packet* encrypted_packet;
        size_t packet_size = l2_raw_packet_get_size(packet);
        size_t full_packet_size = l2_raw_packet_calculate_size(packet_size);

        log_info("Broadcasting packet to all clients");
        log_info("Clients count: %ld", server->accepted_clients);

        for (size_t i = 0; i < server->accepted_clients; i++) {
                conn = server->clients[i];
                client = conn->client;

                encrypted_packet = l2_client_alloc_temp_mem(
                        client,
                        full_packet_size
                );

                memcpy(encrypted_packet, packet, full_packet_size);

                l2_client_send_packet(
                        client,
                        game_service_crypt_packet_encrypt(encrypted_packet, conn->encrypt_key)
                );
        }
}

void l2_server_broadcast_packet
(
        struct L2Server* server,
        struct L2Client* from,
        l2_packet* packet
)
{
        assert(server);
        assert(from);
        assert(packet);

        if (server->accepted_clients - 1 == 0) return;

        struct ConnectionThread* conn;
        struct L2Client* client;
        l2_raw_packet* encrypted_packet;
        size_t packet_size = l2_raw_packet_get_size(packet);
        size_t full_packet_size = l2_raw_packet_calculate_size(packet_size);

        log_info("Broadcasting packet to the rest of the clients");
        log_info("Clients count (ignoring myself): %ld", server->accepted_clients - 1);

        for (size_t i = 0; i < server->accepted_clients; i++) {
                conn = server->clients[i];
                client = conn->client;

                if (client == from) continue;

                encrypted_packet = l2_client_alloc_temp_mem(
                        client,
                        full_packet_size
                );

                memcpy(encrypted_packet, packet, full_packet_size);

                l2_client_send_packet(
                        client,
                        game_service_crypt_packet_encrypt(encrypted_packet, conn->encrypt_key)
                );
        }
}

void l2_server_free(struct L2Server* server)
{
        assert(server);
        if (server->clients) free(server->clients);
        if (server->socket_handler) free(server->socket_handler);
        free(server);
}

struct L2Server* l2_server_create(size_t max_players)
{
        struct L2Server* server = calloc(1, sizeof(struct L2Server));

        if (server == NULL) {
                return NULL;
        }

        server->clients = malloc(sizeof(struct ConnectionThread *) * max_players);
        server->socket_handler = calloc(1, os_socket_handler_size());

        if (!server->clients || !server->socket_handler) {
                l2_server_free(server);
                return NULL;
        }

        server->accepted_clients = 0;

        return server;
}

void l2_server_listen(struct L2Server* server, unsigned short port)
{
        assert(server);

        os_socket_connect(server->socket_handler);
        os_socket_bind(server->socket_handler, port);
        os_socket_listen(server->socket_handler, 3);
}

struct ConnectionThread* l2_server_get_client
(
        struct L2Server* server,
        size_t index
)
{
        assert(server);
        struct ConnectionThread* conn = calloc(1, sizeof(struct ConnectionThread));
        conn->client = calloc(1, l2_client_struct_size());
        server->clients[index] = conn;
        return conn;
}

void l2_server_accept_client
(
        struct L2Server* server,
        l2_server_accept_conn_handler handler
)
{
        assert(server);
        assert(handler);

        struct ConnectionThread* conn = l2_server_get_client(
                server,
                server->accepted_clients
        );

        conn->server = server;

        l2_client_init(conn->client);
        l2_client_accept(conn->client, server->socket_handler);
        server->accepted_clients += 1;
        pthread_create(&conn->thread, NULL, handler, conn);
        //server->accepted_clients -= 1;
        //l2_client_close(client);
}

void l2_server_start
(
        struct L2Server* server,
        unsigned short port,
        l2_server_accept_conn_handler handler
)
{
        assert(server);
        assert(handler);
        l2_server_listen(server, port);
        while(1) l2_server_accept_client(server, handler);
}
