#ifndef L2AUTH_BLOWFISH_KEY_H
#define L2AUTH_BLOWFISH_KEY_H

#include <openssl/blowfish.h>

struct L2BlowfishKey;

struct L2BlowfishKey* l2_blowfish_key_new();
void l2_blowfish_encrypt
(
        struct L2BlowfishKey* key,
        unsigned char* src,
        unsigned char* dest
);
void l2_blowfish_decrypt
(
        struct L2BlowfishKey* key,
        unsigned char* src,
        unsigned char* dest
);

#endif
