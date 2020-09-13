#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>

void server_init
(unsigned short port, size_t max_players);

void *server_start
(void *p);

#endif
