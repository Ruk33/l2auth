#ifndef LIB_GAME_SERVER_H
#define LIB_GAME_SERVER_H

#include <stddef.h>
#include <sys/types.h>
#include <host.h>
#include <db/conn.h>

typedef void (*msg_to_host_cb)(int type, void *);

typedef struct {
        void *         memory_pool;
        size_t         memory_pool_size;
        msg_to_host_cb msg_to_host;
} game_server_t;

/**
 * To be called from host at initialization.
 */
game_server_t *game_server_init(void *memory, size_t size, msg_to_host_cb cb);

/**
 * "Allocate" memory. Since the host sends us
 * a chunk of memory at initialization (game_server_init)
 * the memory is guaranteed to be returned.
 */
void *game_server_alloc_mem(game_server_t *gs, size_t n);

/**
 * Free allocated memory with game_server_alloc_mem.
 */
void game_server_free_mem(game_server_t *gs, void *m);

/**
 * Ask host to log msg.
 * Assume it will be q'd and handled when possible.
 */
void game_server_log(game_server_t *gs, char *msg);

/**
 * Send response to host.
 * Assume it will be q'd to be handled when possible.
 */
void game_server_send_response(game_server_t *gs, int socket, void *r, size_t n);

/**
 * Intended to be called from host.
 * type = new request, timer tick, etc.
 */
void game_server_request(game_server_t *gs, int type, void *payload, ssize_t n);

#endif
