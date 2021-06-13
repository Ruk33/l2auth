#ifndef INCLUDE_GS_PACKET_PROTOCOL_VERSION_H
#define INCLUDE_GS_PACKET_PROTOCOL_VERSION_H

#include "util.h"
#include "packet.h"

typedef struct {
        byte_t content[9];
} gs_packet_protocol_version_t;

void gs_packet_protocol_version(gs_packet_protocol_version_t *src);

void gs_packet_protocol_version_pack(
        packet_t *dest,
        gs_packet_protocol_version_t *src);

#endif
