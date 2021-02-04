#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "socket.h"
#include "game_server_lib.h"
#include "memory.h"
#include "server.h"

struct Server {
        void *data;
};

typedef struct Server server_t;

static int handle_connection(int fd, void *data)
{
        server_t *server = NULL;
        int new_conn_result = 0;
        int client_socket = 0;

        assert(data);

        server = data;

        printf("Accepting new connection.\n");
        client_socket = socket_accept(fd);

        if (client_socket == -1) {
                printf("Failed to accept new connection.\n");
                return -1;
        }

        new_conn_result = game_server_lib_new_connection(client_socket, server->data);

        if (new_conn_result == -1) {
                printf("Failed to handle new connection.\n");
                return -1;
        }

        return client_socket;
}

static void handle_request(int fd, void *data, unsigned char *request, size_t request_size)
{
        server_t *server = NULL;

        assert(data);
        assert(request);
        assert(request_size);

        server = data;

        printf("Handling new request from %d of size %ld.\n", fd, request_size);

        game_server_lib_handle_request(
                fd,
                request,
                request_size,
                server->data,
                &memory_alloc,
                &memory_free,
                &socket_send,
                &socket_close
        );
}

static void handle_disconnect(int fd, void *data)
{
        server_t *server = data;

        assert(data);

        printf("Client %d disconnected.\n", fd);
        game_server_lib_handle_disconnect(fd, server->data);
}

int server_start(unsigned short port, size_t max_connections)
{
        server_t server = {0};
        socket_conn_t conn = {0};
        int handle_requests_result = 0;

        assert(port);
        assert(max_connections);

        conn.fd = socket_create(port, max_connections);
        conn.data = &server;
        conn.on_connect = &handle_connection;
        conn.on_request = &handle_request;
        conn.on_disconnect = &handle_disconnect;

        if (conn.fd == -1) {
                printf("Unable to create socket.\n");
                return EXIT_FAILURE;
        }

        server.data = game_server_lib_handle_init(&memory_alloc, &memory_free);

        if (!server.data) {
                printf("Failed to initialize game server.\n");
                return EXIT_FAILURE;
        }

        printf("Waiting for requests...\n");
        handle_requests_result = socket_handle_requests(&conn);

        if (handle_requests_result == -1) {
                printf("Unable to handle requests.\n");
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
}
