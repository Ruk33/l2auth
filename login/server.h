#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>

/**
 * Start the server.
 * On success, the thread will be blocked and 
 * 0 will be returned when the server is stopped.
 * On error, -1 will be returned.
 */
int server_start(unsigned short port, size_t max_connections);

#endif
