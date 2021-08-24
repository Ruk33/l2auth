#ifndef INCLUDE_LS_SESSION_H
#define INCLUDE_LS_SESSION_H

#include "util.h"
#include "os_io.h"
#include "packet.h"

struct ls_rsa;

struct ls_blowfish;

struct ls_session {
        size_t id;
        struct os_io *socket;
        struct ls_blowfish *blowfish;
        struct ls_rsa *rsa;
        i32_t playOK1;
        i32_t playOK2;
};

struct ls_session *ls_session_new(struct os_io *socket);

void ls_session_free(struct ls_session *session);

struct ls_session *ls_session_find(struct os_io *socket);

void ls_session_rsa_modulus(struct ls_session *session, byte_t *dest);

void ls_session_encrypt_packet(
        struct ls_session *session,
        byte_t *dest,
        packet_t *src);

void ls_session_decrypt_packet(
        struct ls_session *session,
        packet_t *dest,
        byte_t *src);

#endif
