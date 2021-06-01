#ifndef INCLUDE_PACKET_AUTH_REQUEST_H
#define INCLUDE_PACKET_AUTH_REQUEST_H

#include "l2_string.h"
#include "packet.h"

typedef struct {
        l2_string_t username[28];

        int playOK1;
        int playOK2;
        int loginOK1;
        int loginOK2;
} packet_auth_request_t;

void packet_auth_request_unpack(packet_auth_request_t *dest, packet_t *src);

#endif
