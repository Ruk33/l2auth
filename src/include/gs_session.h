#ifndef INCLUDE_GS_SESSION_H
#define INCLUDE_GS_SESSION_H

#include "config.h"
#include "util.h"
#include "gs_types.h"
#include "packet.h"
#include "platform.h"
#include "gs_client_packets.h"

struct gs_session *gs_session_new(struct gs_state *state,
                                  struct platform_socket *socket);

struct gs_session *gs_session_find(struct gs_state *state,
                                   struct platform_socket *socket);

// Mark the connection as encrypted.
// This function AFFECTS gs_session_decrypt.
void gs_session_encrypt_conn(struct gs_session *session);

void gs_session_update_auth(struct gs_session *dest,
                            struct gs_packet_auth_request *src);

void gs_session_send_packet(struct gs_state *gs,
                            struct gs_session *session,
                            packet_t *src);

void gs_session_encrypt(struct gs_session *session,
                        byte_t *dest,
                        packet_t *src);

// Decrypt packet if connection is encrypted.
// If not encrypted, src gets copied to dest.
void gs_session_decrypt(struct gs_session *session,
                        packet_t *dest,
                        byte_t *src);

void gs_session_disconnected(struct gs_state *gs, struct gs_session *session);

void gs_session_disconnect(struct gs_state *state, struct gs_session *session);

#endif
