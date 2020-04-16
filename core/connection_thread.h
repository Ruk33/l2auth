#ifndef L2AUTH_CONNECTION_THREAD_H
#define L2AUTH_CONNECTION_THREAD_H

#include <pthread.h>
#include <core/l2_server.h>
#include <core/l2_client.h>

struct ConnectionThread
{
        pthread_t thread;
        struct L2Server* server;
        struct L2Client* client;
        int is_encrypted;
        unsigned char encrypt_key[8];
        unsigned char decrypt_key[8];
};


#endif
