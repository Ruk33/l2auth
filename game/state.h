#ifndef L2AUTH_GAME_STATE_H
#define L2AUTH_GAME_STATE_H

#include <os/socket.h>
#include <core/memory.h>

struct GameState {
        os_socket_handler* server_socket;
        memory* memory;
};

#endif
