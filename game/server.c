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

static int handle_connection(int fd, void *data)
{
        assert(fd > 0);
        assert(data);

        struct Server *server = data;
        int client_socket = 0;

        printf("Accepting new connection.\n");

        client_socket = socket_accept(fd);

        if (client_socket == -1)
        {
                printf("Failed to accept new connection.\n");
                return -1;
        }

        if (game_server_lib_new_connection(client_socket, server->data) == -1)
        {
                printf("Failed to handle new connection.\n");
                return -1;
        }

        return client_socket;
}

static void handle_request(int fd, void *data, unsigned char *request, size_t request_size)
{
        assert(fd > 0);
        assert(data);
        assert(request);
        assert(request_size);

        struct Server *server = data;

        printf("Handling new request from %d of size %ld.\n", fd, request_size);

        // request_manager_add(server->data, fd, request, request_size);

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
        assert(fd > 0);
        assert(data);

        struct Server *server = data;

        printf("Client %d disconnected.\n", fd);
        game_server_lib_handle_disconnect(fd, server->data);
}

int server_start(unsigned short port, size_t max_connections)
{
        assert(port);
        assert(max_connections);

        int server_socket = socket_create(port, max_connections);
        struct Server server = {0};
        int handle_requests_result = 0;

        if (server_socket == -1)
        {
                printf("Unable to create socket.\n");
                return EXIT_FAILURE;
        }

        server.data = game_server_lib_handle_init(&memory_alloc, &memory_free);

        if (!server.data)
        {
                printf("Failed to initialize game server.\n");
                return EXIT_FAILURE;
        }

        printf("Waiting for requests...\n");

        handle_requests_result = socket_handle_requests(
                server_socket,
                &server,
                &handle_connection,
                &handle_request,
                &handle_disconnect
        );

        if (handle_requests_result == -1)
        {
                printf("Unable to handle requests.\n");
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
}
