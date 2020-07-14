#ifndef L2AUTH_GAME_CLIENT_CONNECTION_H
#define L2AUTH_GAME_CLIENT_CONNECTION_H

#include <os/socket.h>
#include <os/memory.h>

struct GameClientConnection {
        os_socket_handler* socket;
        memory* memory;
        void* conn;
};

#endif
