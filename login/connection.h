#ifndef L2AUTH_LOGIN_CONNECTION_H
#define L2AUTH_LOGIN_CONNECTION_H

#include <pthread.h>
#include <login/server.h>
#include <login/client.h>

struct LoginConnection
{
        pthread_t thread;
        struct LoginServer* server;
        struct LoginClient* client;
};


#endif
