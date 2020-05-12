#include <assert.h>
#include <stdlib.h>
#include <log/log.h>
#include <os/socket.h>
#include <login/handler/client.h>
#include <login/client.h>
#include <login/connection.h>
#include "server.h"

struct LoginServer
{
        os_socket_handler* socket_handler;
        size_t accepted_clients;
        size_t max_players;
        struct LoginConnection** clients;
};

void login_server_free(struct LoginServer* server)
{
        assert(server);
        if (server->clients) free(server->clients);
        if (server->socket_handler) free(server->socket_handler);
        free(server);
}

struct LoginServer* login_server_create(size_t max_players)
{
        struct LoginServer* server = calloc(1, sizeof(struct LoginServer));

        if (!server) return NULL;

        server->clients = malloc(sizeof(struct LoginConnection *) * max_players);
        server->socket_handler = calloc(1, os_socket_handler_size());

        if (!server->clients || !server->socket_handler) {
                login_server_free(server);
                return NULL;
        }

        server->accepted_clients = 0;
        server->max_players = max_players;

        return server;
}

void login_server_listen(struct LoginServer* server, unsigned short port)
{
        assert(server);

        os_socket_connect(server->socket_handler);
        os_socket_bind(server->socket_handler, port);
        os_socket_listen(server->socket_handler, server->max_players);
}

struct LoginConnection* login_server_get_client
(
        struct LoginServer* server,
        size_t index
)
{
        assert(server);
        struct LoginConnection* conn = calloc(1, sizeof(struct LoginConnection));
        conn->client = calloc(1, login_client_struct_size());
        server->clients[index] = conn;
        return conn;
}

void login_server_accept_client
(
        struct LoginServer* server,
        void* handler
)
{
        assert(server);
        assert(handler);

        struct LoginConnection* conn = login_server_get_client(
                server,
                server->accepted_clients
        );

        conn->server = server;

        login_client_init(conn->client);
        login_client_accept(conn->client, server->socket_handler);
        server->accepted_clients += 1;
        pthread_create(&conn->thread, NULL, handler, conn);
        //server->accepted_clients -= 1;
        //l2_client_close(client);
}

void login_server_start_or_die(unsigned short port, size_t max_players)
{
        struct LoginServer* server = login_server_create(max_players);

        log_info("Starting loginserver");

        if (!server) {
                log_fatal("Not able to allocate memory for loginserver");
                exit(1);
        }

        login_server_listen(server, port);
        while(1) login_server_accept_client(server, login_handler_client);
}
