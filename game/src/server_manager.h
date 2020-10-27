#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <stdlib.h>

/**
 * NOTE, make sure to call this function before using the manager
 */
void server_manager_init(unsigned short port, size_t max_players);

void *server_manager_start(void *p);

void *server_get_data(void);

#endif
