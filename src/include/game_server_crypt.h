#ifndef INCLUDE_GAME_SERVER_CRYPT_H
#define INCLUDE_GAME_SERVER_CRYPT_H

#include "util.h"
#include "packet.h"

void game_server_encrypt(byte_t *key, byte_t *dest, packet_t *src);

void game_server_decrypt(byte_t *key, packet_t *dest, byte_t *src);

#endif
