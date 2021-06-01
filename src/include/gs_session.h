#ifndef INCLUDE_GS_SESSION_H
#define INCLUDE_GS_SESSION_H

#include "util.h"
#include "socket.h"
#include "packet.h"

typedef struct {
        socket_t *socket;

        byte_t encrypt_key[8];
        byte_t decrypt_key[8];

        int playOK1;
        int playOK2;
        int loginOK1;
        int loginOK2;

        char username[16];
        int  conn_encrypted;
} gs_session_t;

gs_session_t *gs_session_new(socket_t *socket);

gs_session_t *gs_session_find(socket_t *socket);

void gs_session_encrypt_conn(gs_session_t *session);

void gs_session_encrypt(gs_session_t *session, byte_t *dest, packet_t *src);

void gs_session_decrypt(gs_session_t *session, packet_t *dest, byte_t *src);

#endif
