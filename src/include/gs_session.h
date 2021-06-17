#ifndef INCLUDE_GS_SESSION_H
#define INCLUDE_GS_SESSION_H

#include "config.h"
#include "util.h"
#include "packet.h"
#include "os_io.h"
#include "gs_packet_auth_request.h"

/*
 * These represent the states of a client's session.
 * It's a state machine, which makes it easier to understand
 * the flow and avoid having to make some checks. For instance,
 * if we are in CHARACTER_SELECTION, we know we have went through
 * PROTOCOL_VERSION and AUTH_REQUEST, so we are dealing with
 * a valid session.
 * Keep these states in order.
 */
typedef enum {
        PROTOCOL_VERSION,
        AUTH_REQUEST,
        CHARACTER_SELECTION,
        CREATING_CHARACTER,
        ENTERING_WORLD,
        IN_WORLD,
} gs_session_state_t;

typedef struct {
        u32_t id;
        os_io_t *socket;

        gs_session_state_t state;

        byte_t encrypt_key[8];
        byte_t decrypt_key[8];

        int playOK1;
        int playOK2;
        int loginOK1;
        int loginOK2;

        char username[MAX_USERNAME_SIZE];
        int conn_encrypted;
} gs_session_t;

void gs_session_set(gs_session_t *sessions, size_t *count);

gs_session_t *gs_session_new(os_io_t *socket);

gs_session_t *gs_session_find(os_io_t *socket);

// Mark the connection as encrypted.
// This function AFFECTS gs_session_decrypt.
void gs_session_encrypt_conn(gs_session_t *session);

void gs_session_update_auth(gs_session_t *dest, gs_packet_auth_request_t *src);

void gs_session_encrypt(gs_session_t *session, byte_t *dest, packet_t *src);

// Decrypt packet if connection is encrypted.
// If not encrypted, src gets copied to dest.
void gs_session_decrypt(gs_session_t *session, packet_t *dest, byte_t *src);

#endif
