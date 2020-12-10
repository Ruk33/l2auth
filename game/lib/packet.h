#ifndef LIB_PACKET_H
#define LIB_PACKET_H

#include <stddef.h>
#include "types.h"

/**
 * Lineage 2 packets consist of two headers
 * and the content. Headers being:
 *
 * Packet size (unsigned short)
 * Contains the full size in bytes of the packet.
 * It includes the size of both headers.
 *
 * Packet type (unsigned char)
 * Contains the type of the packet.
 *
 * The rest of the content represents the
 * body of the packet.
 */
typedef unsigned char packet;

typedef unsigned short packet_size;

typedef unsigned char packet_type;

/**
 * Build packet from type and content.
 * dest must be long enough to hold the
 * entire information.
 */
void packet_build(packet *dest, packet_type type, byte *content, size_t content_size);

/**
 * Get the FULL size from a packet
 * built with packet_build. The packet
 * size includes: size header, type header and
 * the body of the packet itself.
 */
packet_size packet_get_size(packet *src);

/**
 * Get the packet type
 * from packet.
 */
packet_type packet_get_type(packet *src);

/**
 * Get the content of the packet.
 */
byte *packet_body(packet *src);

#endif
