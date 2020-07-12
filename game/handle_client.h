#ifndef L2AUTH_GAME_HANDLE_CLIENT_H
#define L2AUTH_GAME_HANDLE_CLIENT_H

#include <stdlib.h>
#include <game/state.h>
#include <game/code.h>

void game_handle_client_listen
(
        struct GameState* game_state,
        struct GameCode* game_code,
        unsigned short port,
        size_t max_clients
);

#endif
