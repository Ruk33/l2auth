#ifndef L2AUTH_BUILD_RSA_KEY_C
#define L2AUTH_BUILD_RSA_KEY_C

#include <openssl/bn.h>
#include <openssl/rsa.h>

struct l2_rsa_key {
  RSA* rsa_key;
};

#define L2_RSA_KEY_DEFAULT_SIZE_IN_BITS 1024
#define L2_RSA_KEY_DEFAULT_E 65537

void l2_rsa_key_build(struct l2_rsa_key* key, int key_size_in_bytes, unsigned long e)
{
    key->rsa_key = RSA_generate_key(key_size_in_bytes, e, 0, 0);
}

void l2_rsa_key_modulus(struct l2_rsa_key* key, unsigned char* dest)
{
    BN_bn2bin(key->rsa_key->n, dest);
}

int l2_rsa_key_size(struct l2_rsa_key* key)
{
    return RSA_size(key->rsa_key);
}

#endif //L2AUTH_BUILD_RSA_KEY_C