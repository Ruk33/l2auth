#ifndef INCLUDE_LS_CRYPT_H
#define INCLUDE_LS_CRYPT_H

#include "util.h"
#include "packet.h"
#include "ls_blowfish.h"

void ls_encrypt(ls_blowfish_t *k, byte_t *dest, packet_t *src);

void ls_decrypt(ls_blowfish_t *k, packet_t *dest, byte_t *src);

#endif
