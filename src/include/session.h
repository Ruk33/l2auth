#ifndef INCLUDE_SESSION_H
#define INCLUDE_SESSION_H

#include "socket.h"
#include "blowfish.h"
#include "rsa.h"

typedef struct {
        socket_t *socket;

        blowfish_t *blowfish;

        rsa_t *rsa;

        int playOK1;
        int playOK2;
} session_t;

session_t *session_new(socket_t *socket);

session_t *session_find(socket_t *socket);

#endif
