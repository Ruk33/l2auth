#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include "socket.h"
#include "handler/new_connection.h"
#include "handler/new_request.h"
#include "handler/disconnect.h"
#include "storage/server_manager.h"
#include "storage/game_server_manager.h"
#include "storage/game_server.h"
#include "server.h"

static int handle_new_connection(int server_socket, void *server_manager)
{
        int client_socket = socket_accept(server_socket);
        printf("New client with id %d was accepted.\n", client_socket);
        handler_new_connection(server_manager, client_socket);
        return client_socket;
}

static void handle_new_request(int client_socket, void *server_manager, unsigned char *request, size_t request_size)
{
        printf("New request from client %d...\n", client_socket);
        printf("Request size: %ld\n", request_size);
        handler_new_request(server_manager, client_socket, request, request_size);
}

static void handle_disconnect(int client_socket, void *server_manager)
{
        printf("Client with id %d was disconnected.\n", client_socket);
        handler_disconnect(server_manager, client_socket);
}

int server_start(unsigned short port, size_t max_connections)
{
        assert(port);
        assert(max_connections);

        int server_socket = socket_create(port, max_connections);
        struct StorageServerManager server_manager = {0};
        struct StorageGameServer bartz = {0};
        struct StorageGameServer sieghardt = {0};
        int server_result = 0;

        if (server_socket == -1)
        {
                printf("Failed to create socket.\n");
                printf("%s.\n", strerror(errno));
                return EXIT_FAILURE;
        }

        /*
         * Add dummy servers
         * 127.0.0.1 works well for Windows
         * 0.0.0.0 works well for Linux
         */

        bartz.id = 1;
        inet_pton(AF_INET, "127.0.0.1", bartz.ip);
        bartz.port = 7777;
        bartz.age_limit = 0x0f;
        bartz.pvp = 0x00;
        bartz.players = 42;
        bartz.max_players = 650;
        bartz.status = 0x01;
        bartz.extra = 0x00;
        bartz.brackets = 0x00;

        sieghardt.id = 2;
        inet_pton(AF_INET, "0.0.0.0", sieghardt.ip);
        sieghardt.port = 7778;
        sieghardt.age_limit = 0x0f;
        sieghardt.pvp = 0x00;
        sieghardt.players = 42;
        sieghardt.max_players = 650;
        sieghardt.status = 0x01;
        sieghardt.extra = 0x00;
        sieghardt.brackets = 0x00;

        storage_server_manager_init(&server_manager);
        storage_game_server_manager_add(&server_manager.game_server_manager, &bartz);
        storage_game_server_manager_add(&server_manager.game_server_manager, &sieghardt);

        printf("Waiting for requests...\n");

        server_result = socket_handle_requests(
                server_socket,
                &server_manager,
                &handle_new_connection,
                &handle_new_request,
                &handle_disconnect
        );

        socket_close(server_socket);

        storage_server_manager_free(&server_manager);

        if (server_result == -1)
        {
                printf("Socket failed to handle requests.\n");
                printf("%s.\n", strerror(errno));
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
}
