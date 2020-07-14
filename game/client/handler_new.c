#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <os/memory.h>
#include <game/state.h>
#include <game/client_connection.h>
#include <game/request/protocol_version/handler.h>
#include <game/server.h>
#include <game/client.h>
#include <game/connection.h>

void game_client_handler_new
(
        struct GameState* game_state,
        struct GameClientConnection* client
)
{
        assert(game_state);
        assert(client);

        struct GameConnection* game_connection;

        unsigned char encrypt_key[] = {
                0x94,
                0x35,
                0x00,
                0x00,
                0xa1,
                0x6c,
                0x54,
                0x87,
        };

        unsigned char decrypt_key[] = {
                0x94,
                0x35,
                0x00,
                0x00,
                0xa1,
                0x6c,
                0x54,
                0x87,
        };

        log_info("Initializing dummy game connection");

        assert(client->memory);
        game_connection = memory_alloc(
                client->memory,
                sizeof(struct GameConnection)
        );

        log_info("Game connection initialized");
        log_info("Initializing dummy game server");

        game_connection->server = memory_alloc(
                client->memory,
                sizeof(struct GameServer)
        );

        game_connection->server->accepted_clients = 1;
        game_connection->server->socket_handler = game_state->server_socket;
        game_connection->server->clients = memory_alloc(
                client->memory,
                sizeof(struct GameConnection *)
        );

        game_connection->client = memory_alloc(
                client->memory,
                sizeof(struct GameClient)
        );

        game_connection->client->socket_handler = client->socket;
        game_client_init(game_connection->client);

        game_connection->server->clients[0] = game_connection;

        game_connection->handler = game_request_protocol_version_handler;

        memcpy(game_connection->encrypt_key, encrypt_key, sizeof(encrypt_key));
        memcpy(game_connection->decrypt_key, decrypt_key, sizeof(decrypt_key));

        client->conn = game_connection;
}
