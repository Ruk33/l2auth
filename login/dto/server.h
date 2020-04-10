#ifndef L2AUTH_LOGIN_SERVER_H
#define L2AUTH_LOGIN_SERVER_H

#include <arpa/inet.h>

struct LoginDtoServer
{
        unsigned char id;
        //in_addr_t ip;
        int port;
        unsigned char age_limit;
        unsigned char pvp;
        unsigned short players_count;
        unsigned short max_players;
        unsigned char status; // 0x00 down, 0x01 up
        int extra; // test & clock
        unsigned char brackets;
};

#endif
