#ifndef L2AUTH_BUILD_RSA_KEY_H
#define L2AUTH_BUILD_RSA_KEY_H

#include <openssl/bn.h>
#include <openssl/rsa.h>

struct L2RSAKey
{
        char* raw_e;
        BIGNUM* e;
        RSA* rsa_key;
};

#define L2_RSA_KEY_DEFAULT_SIZE_IN_BITS 1024

struct L2RSAKey* l2_rsa_key_new();
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
