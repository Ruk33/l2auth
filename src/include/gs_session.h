#ifndef INCLUDE_GS_SESSION_H
#define INCLUDE_GS_SESSION_H

#include "config.h"
#include "util.h"
#include "socket.h"
#include "packet.h"
#include "packet_auth_request.h"

typedef void (*gs_session_save_cb)(void *, size_t);

typedef struct {
        socket_t *socket;

        byte_t encrypt_key[8];
        byte_t decrypt_key[8];

        int playOK1;
        int playOK2;
        int loginOK1;
        int loginOK2;

        char username[MAX_USERNAME_SIZE];
        int conn_encrypted;
} gs_session_t;

void gs_session_set_save_cb(gs_session_save_cb cb);

void gs_session_load(void *sessions, size_t n);

void gs_sessions_save(void);

gs_session_t *gs_session_new(socket_t *socket);

gs_session_t *gs_session_find(socket_t *socket);

// Mark the connection as encrypted.
// This function AFFECTS gs_session_decrypt.
void gs_session_encrypt_conn(gs_session_t *session);

void gs_session_update_auth(gs_session_t *dest, packet_auth_request_t *src);

void gs_session_encrypt(gs_session_t *session, byte_t *dest, packet_t *src);

// Decrypt packet if connection is encrypted.
// If not encrypted, src gets copied to dest.
void gs_session_decrypt(gs_session_t *session, packet_t *dest, byte_t *src);

#endif
