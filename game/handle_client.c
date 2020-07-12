#include <stdlib.h>
#include <assert.h>
#include <log/log.h>
#include <os/socket.h>
#include <core/memory.h>
#include <game/state.h>
#include <game/code.h>
#include <game/client_connection.h>
#include "handle_client.h"

void game_handle_client_request
(
        struct GameState* game_state,
        struct GameCode* game_code,
        struct GameClientConnection* client
)
{
        unsigned char buffer[65535];
        size_t request_size = os_socket_receive(client->socket, buffer, sizeof(buffer));
        int closed_connection = request_size <= 0;

        game_code_load(game_code);

        if (closed_connection) {
                // game_code->on_client_end(game_state, client);
                return;
        }

        game_code->on_client_request(game_state, client, buffer, request_size);
}

void game_handle_client_new(struct GameState* game_state, struct GameCode* game_code)
{
        struct GameClientConnection* client;
        os_socket_handler* socket = memory_alloc(game_state->memory, sizeof(os_socket_handler));

        os_socket_accept(game_state->server_socket, socket);

        log_info("Client accepted");

        client = memory_alloc(
                game_state->memory,
                sizeof(struct GameClientConnection)
        );

        log_info("Game client connection created");

        client->socket = socket;
        client->memory = memory_alloc(game_state->memory, 1024 * 1024);
        assert(client->memory);
        memory_init(client->memory, 1024 * 1024);

        log_info("Client memory initialized");

        // Usually all the memory for each client should be requested
        // from client->memory
        // client->socket = memory_alloc(client->memory, sizeof(os_socket_handler));

        game_code_load(game_code);
        game_code->on_new_client(game_state, client);

        // This should run on a new thread
        while (1) game_handle_client_request(game_state, game_code, client);
}

void game_handle_client_listen
(
        struct GameState* game_state,
        struct GameCode* game_code,
        unsigned short port,
        size_t max_clients
)
{
        game_state->server_socket = memory_alloc(game_state->memory, sizeof(os_socket_handler));

        os_socket_connect(game_state->server_socket);
        os_socket_bind(game_state->server_socket, port);
        os_socket_listen(game_state->server_socket, max_clients);

        log_info("Listening for connections");

        while (1) game_handle_client_new(game_state, game_code);
}
