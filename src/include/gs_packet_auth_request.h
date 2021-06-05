#ifndef INCLUDE_GS_PACKET_AUTH_REQUEST_H
#define INCLUDE_GS_PACKET_AUTH_REQUEST_H

#include "l2_string.h"
#include "packet.h"

typedef struct {
        l2_string_t username[28];

        int playOK1;
        int playOK2;
        int loginOK1;
        int loginOK2;
} gs_packet_auth_request_t;

void gs_packet_auth_request_unpack(gs_packet_auth_request_t *d, packet_t *s);

#endif
