#ifndef INCLUDED_PACKET_GG_AUTH_H
#define INCLUDED_PACKET_GG_AUTH_H

#include "packet.h"

typedef enum {
        PACKET_GG_AUTH_RESPONSE_SKIP = 0x0b,
} packet_gg_auth_response_t;

typedef struct {
        packet_gg_auth_response_t response;
} packet_gg_auth_t;

void packet_gg_auth_pack(packet_t *dest, packet_gg_auth_t *src);

#endif
