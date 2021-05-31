#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>

/**
 * Start listening for socket connections.
 * data refers to the server data initialized
 * by the game server library at initialization.
 * This function will block the thread.
 *
 * NOTE: Should we not block the thread?
 */
void *server_start(void *data);

#endif
