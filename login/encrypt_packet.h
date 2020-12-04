#ifndef ENCRYPT_PACKET_H
#define ENCRYPT_PACKET_H

#include "blowfish.h"

/**
 * Encrypt packet using blowfish and
 * store result in dest.
 */
void encrypt_packet(struct Blowfish *blowfish, unsigned char *dest, unsigned char *packet);

#endif
