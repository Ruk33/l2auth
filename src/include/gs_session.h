#ifndef INCLUDE_GS_SESSION_H
#define INCLUDE_GS_SESSION_H

#include "config.h"
#include "util.h"
#include "gs_types.h"
#include "packet.h"
#include "os_io.h"
#include "gs_packet_auth_request.h"

struct gs_session *gs_session_new(struct gs_state *state, os_io_t *socket);

struct gs_session *gs_session_find(struct gs_state *state, os_io_t *socket);

// Mark the connection as encrypted.
// This function AFFECTS gs_session_decrypt.
void gs_session_encrypt_conn(struct gs_session *session);

void gs_session_update_auth(
        struct gs_session *dest,
        gs_packet_auth_request_t *src);

void gs_session_encrypt(struct gs_session *session, byte_t *dest, packet_t *src);

// Decrypt packet if connection is encrypted.
// If not encrypted, src gets copied to dest.
void gs_session_decrypt(struct gs_session *session, packet_t *dest, byte_t *src);

#endif
