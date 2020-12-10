#ifndef LIB_DECRYPT_PACKET_H
#define LIB_DECRYPT_PACKET_H

#include "headers.h"

/**
 * Decrypt packet using key.
 * Key WILL BE MODIFIED and this is
 * required so new packets can be
 * properly decrypted.
 * The decrypted packet will be
 * stored in dest.
 */
void decrypt_packet(byte *dest, packet *src, size_t src_size, byte *key);

#endif
