#ifndef DECRYPT_PACKET_H
#define DECRYPT_PACKET_H

#include <stdlib.h>
#include "blowfish.h"

/**
 * Decrypt packet using blowfish and store
 * the contents in dest, creating a new, 
 * decrypted packet.
 */
void decrypt_packet(struct Blowfish *blowfish, unsigned char *dest, unsigned char *packet, size_t packet_size);

#endif
