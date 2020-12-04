#ifndef STORAGE_GAME_SERVER_H
#define STORAGE_GAME_SERVER_H

struct StorageGameServer {
        unsigned char id;
        unsigned char ip[4];
        int port;
        unsigned char age_limit;
        unsigned char pvp;
        unsigned short players;
        unsigned short max_players;
        unsigned char status;
        int extra;
        unsigned char brackets;
};

#endif
