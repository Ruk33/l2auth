#ifndef INCLUDED_LS_PACKET_GG_AUTH_H
#define INCLUDED_LS_PACKET_GG_AUTH_H

#include "packet.h"

typedef enum {
        PACKET_GG_AUTH_RESPONSE_SKIP = 0x0b,
} ls_gg_auth_response_t;

typedef struct {
        ls_gg_auth_response_t response;
} ls_packet_gg_auth_t;

void ls_packet_gg_auth_pack(packet_t *dest, ls_packet_gg_auth_t *src);

#endif
