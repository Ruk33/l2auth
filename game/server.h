#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>

/**
 * Start the game server.
 * If everything goes well, the thread
 * will be blocked to listen and handle
 * requests. When finished, 0 will be returned.
 * On error, -1 will be immediately returned.
 */
int server_start(unsigned short port, size_t max_connections);

#endif
