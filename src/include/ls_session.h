#ifndef INCLUDE_LS_SESSION_H
#define INCLUDE_LS_SESSION_H

#include "util.h"
#include "os_io.h"
#include "packet.h"
#include "ls_types.h"

struct ls_session *ls_session_new(struct ls_state *ls, struct os_io *socket);

struct ls_session *ls_session_find(struct ls_state *ls, struct os_io *socket);

void ls_session_rsa_modulus(struct ls_session *session, byte_t *dest);

void ls_session_send_packet(
        struct ls_state *ls,
        struct ls_session *session,
        packet_t *src);

void ls_session_disconnected(struct ls_state *ls, struct ls_session *session);

void ls_session_disconnect(struct ls_state *ls, struct ls_session *session);

void ls_session_encrypt_packet(
        struct ls_session *session,
        byte_t *dest,
        packet_t *src);

void ls_session_decrypt_packet(
        struct ls_session *session,
        packet_t *dest,
        byte_t *src);

#endif
