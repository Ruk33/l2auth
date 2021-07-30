#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

// Maximum amount of clients/players to have in a server.
#define MAX_CLIENTS 30

// Maximum amount of npcs to have in a server.
#define MAX_NPCS 30

// Maximum amount of characters in the world (players + npcs)
#define MAX_CHARACTERS (MAX_CLIENTS + MAX_NPCS)

// Maximum amount of servers to register.
#define MAX_SERVERS 2

// Maximum bytes used for usernames.
#define MAX_USERNAME_SIZE 28

#define TICKS_PER_SECOND (10)
#define MILLIS_IN_TICK (1000 / TICKS_PER_SECOND)

#endif
