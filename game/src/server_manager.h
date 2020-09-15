#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <stdlib.h>

void server_manager_init
(unsigned short port, size_t max_players);

void *server_manager_start
(void *p);

#endif
