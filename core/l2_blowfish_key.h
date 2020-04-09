#ifndef L2AUTH_BLOWFISH_KEY_H
#define L2AUTH_BLOWFISH_KEY_H

#include <stdlib.h>
#include <openssl/blowfish.h>

struct L2BlowfishKey;

size_t l2_blowfish_key_struct_size();
void l2_blowfish_key_init(struct L2BlowfishKey* l2_key);
void l2_blowfish_encrypt
(
        struct L2BlowfishKey* l2_key,
        unsigned char* src,
        unsigned char* dest
);
void l2_blowfish_decrypt
(
        struct L2BlowfishKey* l2_key,
        unsigned char* src,
        unsigned char* dest
);

#endif
