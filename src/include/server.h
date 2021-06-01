#ifndef INCLUDE_SERVER_H
#define INCLUDE_SERVER_H

#include "util.h"

typedef struct {
        u8_t  id;
        u8_t  ip[4];
        int   port;
        u8_t  age_limit;
        u8_t  pvp;
        u16_t players;
        u16_t max_players;
        u8_t  status;
        int   extra;
        u8_t  brackets;
} server_t;

// Register a new server.
void server_add(server_t *server);

// Get all registered servers.
server_t *server_all(void);

// Get count of registered servers.
size_t server_all_count(void);

#endif
