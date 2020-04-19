#include <assert.h>
#include <stdlib.h>
#include <pthread.h>
#include <log/log.h>
#include <core/connection_thread.h>
#include <core/l2_client.h>
#include <os/socket.h>
#include <core/l2_server.h>

struct L2Server {
        os_socket_handler* socket_handler;
        size_t accepted_clients;
};

void l2_server_free(struct L2Server* server)
{
        assert(server);
        if (server->socket_handler) free(server->socket_handler);
        free(server);
}

struct L2Server* l2_server_create(size_t max_players)
{
        struct L2Server* server = calloc(1, sizeof(struct L2Server));

        if (server == NULL) {
                return NULL;
        }

        server->socket_handler = calloc(1, os_socket_handler_size());

        if (server->socket_handler == NULL) {
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

struct L2Client* l2_server_get_client
(
        struct L2Server* server,
        size_t index
)
{
        assert(server);
        /*
         * (franco.montenegro)
         * Disabled until fixed. It only works with 1 conn
         * return server->clients + (index * l2_client_struct_size());
         */
        return calloc(1, l2_client_struct_size());
}

void l2_server_accept_client
(
        struct L2Server* server,
        l2_server_accept_conn_handler handler
)
{
        assert(server);
        assert(handler);

        struct L2Client* client = l2_server_get_client(
                server,
                server->accepted_clients
        );

        struct ConnectionThread* conn;

        if (client == NULL) {
                log_fatal("Could not allocate memory to accept client");
                return;
        }

        conn = calloc(1, sizeof(struct ConnectionThread));

        if (conn == NULL) {
                log_fatal("Could not allocate memory to accept client");
                free(client);
                return;
        }

        conn->server = server;
        conn->client = client;

        l2_client_init(client);
        l2_client_accept(client, server->socket_handler);
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
