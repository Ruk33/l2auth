#ifndef INCLUDE_PACKET_PROTOCOL_VERSION_H
#define INCLUDE_PACKET_PROTOCOL_VERSION_H

#include "util.h"
#include "packet.h"

typedef struct {
        byte_t content[9];
} packet_protocol_version_t;

void packet_protocol_version(packet_protocol_version_t *src);

void packet_protocol_version_pack(packet_t *d, packet_protocol_version_t *s);

#endif
