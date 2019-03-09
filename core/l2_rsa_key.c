#ifndef L2AUTH_BUILD_RSA_KEY_C
#define L2AUTH_BUILD_RSA_KEY_C

#include <openssl/bn.h>
#include <openssl/rsa.h>

struct l2_rsa_key {
        RSA* rsa_key;
};

#define L2_RSA_KEY_DEFAULT_SIZE_IN_BITS 1024
#define L2_RSA_KEY_DEFAULT_E 65537

void l2_rsa_key_build(struct l2_rsa_key* key)
{
    const char e_char[] = "65537";
    BIGNUM* e = BN_new();
    BN_dec2bn(&e, e_char);
    key->rsa_key = RSA_new();
    RSA_generate_key_ex(
        key->rsa_key,
        L2_RSA_KEY_DEFAULT_SIZE_IN_BITS,
        e,
        NULL
    );
    BN_free(e);
}

void l2_rsa_key_modulus(struct l2_rsa_key* key, unsigned char* dest)
{
        const BIGNUM *n;
        RSA_get0_key(key->rsa_key, &n, NULL, NULL);
        BN_bn2bin(n, dest);
}

int l2_rsa_key_size(struct l2_rsa_key* key)
{
        return RSA_size(key->rsa_key);
}

#endif //L2AUTH_BUILD_RSA_KEY_C
