#ifndef L2AUTH_LOGIN_GAME_CRYPT_H
#define L2AUTH_LOGIN_GAME_CRYPT_H

#include <stdlib.h>

void game_crypt_encrypt
(
        unsigned char* data,
        unsigned int len,
        unsigned char* key
);
void game_crypt_decrypt
(
        unsigned char* data,
        unsigned int len,
        unsigned char* key
);
int game_crypt_checksum(unsigned char* data, size_t len);

#endif
