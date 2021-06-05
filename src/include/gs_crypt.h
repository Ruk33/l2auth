#ifndef INCLUDE_GS_CRYPT_H
#define INCLUDE_GS_CRYPT_H

#include "util.h"
#include "packet.h"

void gs_encrypt(byte_t *key, byte_t *dest, packet_t *src);

void gs_decrypt(byte_t *key, packet_t *dest, byte_t *src);

#endif
