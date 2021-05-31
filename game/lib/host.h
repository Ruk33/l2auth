#ifndef LIB_CONNECTION_H
#define LIB_CONNECTION_H

#include <stdlib.h>

/**
 * Dummy struct to prevent cyclic dependencies.
 * For example, game_server requiring state_machine/handle
 * and vice versa. Instead of state_machine/handle accepting
 * a game_server_t type, we just pass a host_t.
 */

typedef struct {
        void *gs;
        void *(*alloc_mem)(void *gs, size_t n);
        void (*free_mem)(void *gs, void *m);
        void (*log)(void *gs, char *msg);
        void (*send_response)(void *gs, int socket, void *r, size_t n);
} host_t;

#endif
