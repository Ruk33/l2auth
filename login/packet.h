#ifndef PACKET_H
#define PACKET_H

#include <stdlib.h>

/**
 * Build packet from type and content.
 * dest must be long enough to hold the
 * entire information.
 */
void packet_build(unsigned char *dest, unsigned char type, unsigned char *content, size_t content_size);

/**
 * Get the FULL size from a packet
 * built with packet_build. The packet
 * size includes: size header, type header and
 * the body of the packet itself.
 */
unsigned short packet_size(unsigned char *packet);

/**
 * Get the packet type
 * from packet.
 */
unsigned char packet_type(unsigned char *packet);

/**
 * Get the content of the packet.
 */
unsigned char *packet_body(unsigned char *packet);

#endif
