#ifndef L2AUTH_BUILD_RSA_KEY_H
#define L2AUTH_BUILD_RSA_KEY_H

#include <stdlib.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>

struct L2RSAKey;

#define L2_RSA_KEY_DEFAULT_SIZE_IN_BITS 1024

size_t l2_rsa_key_struct_size();

void l2_rsa_key_init(struct L2RSAKey* key);

void l2_rsa_key_scramble_modulo(unsigned char* n);

void l2_rsa_key_modulus(struct L2RSAKey* key, unsigned char* dest);

int l2_rsa_key_size(struct L2RSAKey* key);

int l2_rsa_key_decrypt
(
        struct L2RSAKey* key,
        unsigned char* src,
        unsigned char* dest
);

#endif
