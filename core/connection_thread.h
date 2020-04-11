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
};


#endif
