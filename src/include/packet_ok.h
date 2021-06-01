#ifndef INCLUDE_PACKET_OK_H
#define INCLUDE_PACKET_OK_H

#include "util.h"
#include "packet.h"

typedef struct {
        int    loginOK1;
        int    loginOK2;
        byte_t after_key[39]; // Unknown
} packet_ok_t;

void packet_ok_pack(packet_t *dest, packet_ok_t *src);

#endif
