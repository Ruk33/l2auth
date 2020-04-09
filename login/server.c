#ifndef L2AUTH_LOGIN_SERVER_C
#define L2AUTH_LOGIN_SERVER_C

#include <assert.h>
#include <stdlib.h>
#include <core/l2_socket.h>
#include <core/l2_client.h>
#include <socket_strategy/socket_strategy_linux.h>
#include <login/handler/client.h>
#include <login/server.h>

struct LoginServer {
        struct L2Socket socket;
        struct L2SocketStrategy socket_strategy;
        size_t accepted_clients;

        /*
         * (franco.montenegro)
         * I'm sure there is a better way of pre-allocating
         * a bunch of structs with unkown type, but for the time
         * being, this will do
         */
        void* clients;
        void* clients_memory;
};

void login_server_free(struct LoginServer* server)
{
        assert(server);
        if (server->clients) free(server->clients);
        if (server->clients_memory) free(server->clients_memory);
        free(server);
}

struct LoginServer* login_server_create
(
        size_t max_players,
        size_t mem_in_bytes_per_player
)
{
        struct LoginServer* server = calloc(1, sizeof(struct LoginServer));

        if (server == NULL) {
                return NULL;
        }

        server->accepted_clients = 0;

        server->clients = calloc(max_players, l2_client_struct_size());
        server->clients_memory = calloc(max_players, mem_in_bytes_per_player);

        if (server->clients == NULL || server->clients_memory == NULL) {
                login_server_free(server);
                return NULL;
        }

        return server;
}

void login_server_listen(struct LoginServer* server, unsigned short port)
{
        assert(server);
        /*
         * (franco.montenegro)
         * I don't like how we are forcing sockets to be
         * linux, why does the server knows about this?
         */
        socket_strategy_linux(&server->socket_strategy);
        l2_socket_connect(&server->socket, &server->socket_strategy);
        l2_socket_bind(&server->socket, port);
        l2_socket_listen(&server->socket);
}

struct L2Client* login_server_get_client
(
        struct LoginServer* server,
        size_t index
)
{
        assert(server);
        return server->clients + (index * l2_client_struct_size());
}

void login_server_accept_client(struct LoginServer* server)
{
        assert(server);

        struct L2Client* client = login_server_get_client(
                server,
                server->accepted_clients
        );

        l2_client_init(client);
        l2_client_accept(client, &server->socket);
        server->accepted_clients += 1;

        login_handler_client(server, client);
}

#endif
