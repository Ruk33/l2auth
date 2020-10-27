#include <assert.h>
#include <stdlib.h>
#include <log/log.h>
#include <os/socket.h>
#include "response_manager.h"
#include "connection_manager.h"
#include "memory_manager.h"
#include "game_server_lib.h"
#include "server_manager.h"

struct Server
{
        unsigned short port;
        size_t max_players;
        os_socket_handler *socket;
        void *data;
};

static struct Server *server = NULL;

static void server_manager_accept_connection(void)
{
        assert(server);
        assert(server->socket);

        os_socket_handler *client_socket = NULL;

        client_socket = memory_manager_alloc(os_socket_handler_size());

        os_socket_accept(server->socket, client_socket);
        connection_manager_new_conn(server->data, client_socket);
}

static void server_manager_listen_for_connections(void)
{
        assert(server);
        assert(server->socket);
        assert(server->port > 0);
        assert(server->max_players > 0);

        log_info("Starting gameserver");
        log_info(
            "Listening on port %d for a maximum of %d connections",
            server->port,
            server->max_players);

        os_socket_connect(server->socket);
        os_socket_bind(server->socket, server->port);
        os_socket_listen(server->socket, server->max_players);

        while (1)
        {
                log_info("Waiting for connections...");
                server_manager_accept_connection();
                log_info("Connection accepted!");
        }

        os_socket_close(server->socket);
}

void server_manager_init(unsigned short port, size_t max_players)
{
        server = memory_manager_alloc(sizeof(*server));
        server->port = port;
        server->max_players = max_players;
        server->socket = memory_manager_alloc(os_socket_handler_size());
        server->data = game_server_lib_init_server(&response_manager_enqueue);
}

void *server_manager_start(void *p)
{
        assert(server);

        server_manager_listen_for_connections();

        memory_manager_free(server->socket);
        memory_manager_free(server);

        return NULL;
}

void *server_get_data(void)
{
        assert(server);
        return server->data;
}
