#ifndef L2AUTH_LOGIN_SERVER_C
#define L2AUTH_LOGIN_SERVER_C

#include <arpa/inet.h>

struct Server
{
        unsigned char id;
        in_addr_t ip;
        int port;
        unsigned char age_limit;
        unsigned char pvp;
        unsigned short players_count;
        unsigned short max_players;
        unsigned char test;
};

#endif
