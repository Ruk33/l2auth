#ifndef LIB_ENCRYPT_PACKET_H
#define LIB_ENCRYPT_PACKET_H

#include "headers.h"

/**
 * Encrypt packet using key.
 * After encryption, the key WILL
 * CHANGE. This is required so further
 * packets can be properly encrypted.
 */
void encrypt_packet(byte *dest, packet *src, size_t src_size, byte *key);

#endif
