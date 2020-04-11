#ifndef L2AUTH_BUILD_RSA_KEY_C
#define L2AUTH_BUILD_RSA_KEY_C

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <core/l2_rsa_key.h>

struct L2RSAKey
{
        char raw_e[6];
        BIGNUM* e;
        RSA* rsa_key;
};

size_t l2_rsa_key_struct_size()
{
        return sizeof(struct L2RSAKey);
}

void l2_rsa_key_init(struct L2RSAKey* key)
{
        assert(key);

        char raw_e[] = "65537";
        size_t raw_e_size = sizeof(raw_e);

        key->e = BN_new();
        key->rsa_key = RSA_new();

        memcpy(key->raw_e, raw_e, raw_e_size);
        BN_dec2bn(&key->e, key->raw_e);

        RSA_generate_key_ex(
                key->rsa_key,
                L2_RSA_KEY_DEFAULT_SIZE_IN_BITS,
                key->e,
                NULL
        );
}

void l2_rsa_key_scramble_modulo(unsigned char* n)
{
        unsigned char temp;
        int i;

        for (i = 0; i < 4; i ++) {
                temp = n[0x00 + i];
                n[0x00 + i] = n[0x4d + i];
                n[0x4d + i] = temp;
        };

        // step 2 xor first 0x40 bytes with last 0x40 bytes 
        for (i = 0; i < 0x40; i++) {
                n[i] = (unsigned char) (n[i] ^ n[0x40 + i]);
        };

        // step 3 xor bytes 0x0d-0x10 with bytes 0x34-0x38 
        for (i = 0; i < 4; i ++) {
                n[0x0d + i] = (unsigned char) (n[0x0d + i] ^ n[0x34 + i]);
        };

        // step 4 xor last 0x40 bytes with first 0x40 bytes 
        for (i = 0; i < 0x40; i ++) {
                n[0x40 + i] = (unsigned char) (n[0x40 + i] ^ n[i]);
        };
};

void l2_rsa_key_modulus(struct L2RSAKey* key, unsigned char* dest)
{
        assert(key);
        assert(dest);

        const BIGNUM *n;

        RSA_get0_key(key->rsa_key, &n, NULL, NULL);
        BN_bn2bin(n, dest);
        l2_rsa_key_scramble_modulo(dest);
}

int l2_rsa_key_size(struct L2RSAKey* key)
{
        assert(key);
        return RSA_size(key->rsa_key);
}

int l2_rsa_key_decrypt
(
        struct L2RSAKey* key,
        unsigned char* src,
        unsigned char* dest
)
{
        return RSA_private_decrypt(
                l2_rsa_key_size(key),
                src,
                dest,
                key->rsa_key,
                RSA_NO_PADDING
        );
}

#endif
