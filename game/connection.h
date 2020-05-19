#ifndef L2AUTH_GAME_CONNECTION_H
#define L2AUTH_GAME_CONNECTION_H

#include <pthread.h>
#include <game/server.h>
#include <game/client.h>

struct GameConnection
{
        pthread_t thread;
        struct GameServer* server;
        struct GameClient* client;
        int is_encrypted;
        unsigned char encrypt_key[8];
        unsigned char decrypt_key[8];
        void (*handler)(struct GameRequest*);
};

#endif
