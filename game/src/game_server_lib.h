#ifndef GAME_SERVER_LIB_H
#define GAME_SERVER_LIB_H

#include <stdlib.h>
#include <os/memory.h>
#include <os/socket.h>

/*
 * Game server lib refers to the code located in the lib folder
 * This code gets dynamically loaded as a library, allowing for
 * hot code reload. This way, changes in the game server can be
 * push and will be reflected live, without having to shutdown
 * the server
 * All handlers/actions will make sure the latest version of 
 * the lib is loaded before performing the action
 */

struct GameServerLib;

typedef void (*send_reponse_cb)(int client_id, unsigned char *buf, size_t buf_size);

/**
 * NOTE, make sure to call this function before using it
 */
void game_server_lib_init(void);

void game_server_lib_tick_server(void *server_data);

/**
 * Fuction called for initializing the server. Useful for
 * initializing the world and any other task needed to run
 * at init
 */
void *game_server_lib_init_server(send_reponse_cb s);

/**
 * Function called when a new client/connection gets accepted
 */
void game_server_lib_new_conn(void *server_data, int client_id);

/**
 * Handle/notify request made by client (packet)
 */
void game_server_lib_request(void *server_data, int client_id, unsigned char *buf, size_t buf_size);

/**
 * When a connection/client gets disconnected or the
 * connection is closed
 */
void game_server_lib_disconnect(void *server_data, int client_id);

/**
 * Release any memory allocated by the manager
 */
void game_server_lib_finish(void);

#endif
