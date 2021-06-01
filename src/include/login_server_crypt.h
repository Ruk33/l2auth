#ifndef INCLUDE_LOGIN_SERVER_CRYPT_H
#define INCLUDE_LOGIN_SERVER_CRYPT_H

#include "util.h"
#include "packet.h"
#include "blowfish.h"

void login_server_encrypt(blowfish_t *k, byte_t *dest, packet_t *src);

void login_server_decrypt(blowfish_t *k, packet_t *dest, byte_t *src);

#endif
