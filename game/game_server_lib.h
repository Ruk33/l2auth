#ifndef GAME_SERVER_LIB_H
#define GAME_SERVER_LIB_H

#include <stddef.h>
#include <sys/types.h>

/**
 * The game server's logic is implemented
 * as a library. This means that we
 * can load code on demand (hot code reload)
 * without having to restart the entire
 * server.
 *
 * The development flow then, goes as follow:
 * - Run the game server
 * - Make a change in the lib folder (game server's logic)
 * - Compile only the changes in the lib folder
 * - Watch it work without having to shutdown the server
 */

/**
 * Initialize game server and
 * return any memory the handler
 * allocates. NULL can be returned.
 */
void *game_server_lib_handle_init(
        void * memory,
        size_t size,
        void (*msg_to_host_cb)(int type, void *));

/**
 * Handle new connection.
 * On error, -1 will be returned.
 */
int game_server_lib_new_connection(int fd, void *data);

/**
 * Handle request.
 * On success, 1 will be returned.
 * On error, -1 will be returned. This could
 * mean that the function handler couldn't
 * be loaded.
 */
int game_server_lib_handle_request(
        int            fd,
        unsigned char *request,
        ssize_t        request_size,
        void *         data);

/**
 * Handle client being disconnected.
 * On success, 1 will be returned.
 * On error, -1 will be returned. This could
 * mean that the function handler couldn't
 * be loaded.
 */
int game_server_lib_handle_disconnect(int fd, void *data);

/**
 * Handle timer tick.
 */
void game_server_lib_handle_timer_tick(double delta, void *data);

#endif
