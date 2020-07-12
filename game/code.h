#ifndef L2AUTH_GAME_CODE_H
#define L2AUTH_GAME_CODE_H

#include <stdlib.h>
#include <os/socket.h>
#include <game/state.h>
#include <game/client_connection.h>

typedef void (*new_client_handler)(struct GameState* game_state, struct GameClientConnection* client);
typedef void (*client_request_handler)(struct GameState* game_state, struct GameClientConnection* client, void* request, size_t request_size);
typedef void (*end_client_handler)(struct GameState* game_state, struct GameClientConnection* client);

struct GameCode {
        void* library;
        new_client_handler on_new_client;
        client_request_handler on_client_request;
        end_client_handler on_client_end;
};

void game_code_load(struct GameCode* game_code);

#endif
