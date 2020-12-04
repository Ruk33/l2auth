#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include "rsa.h"

void rsa_init(struct Rsa *rsa)
{
        assert(rsa);

        rsa->e = BN_new();
        rsa->key = RSA_new();

        BN_dec2bn(&rsa->e, "65537");
        RSA_generate_key_ex(rsa->key, 1024, rsa->e, NULL);
}

void rsa_free(struct Rsa *rsa)
{
        assert(rsa);
        BN_free(rsa->e);
        RSA_free(rsa->key);
}

int rsa_size(struct Rsa* rsa)
{
        assert(rsa);
        return RSA_size(rsa->key);
}

static void rsa_scramble_modulo(unsigned char *n)
{
        unsigned char temp = 0;
        int i = 0;

        for (i = 0; i < 4; i ++)
        {
                temp = n[0x00 + i];
                n[0x00 + i] = n[0x4d + i];
                n[0x4d + i] = temp;
        };

        // step 2 xor first 0x40 bytes with last 0x40 bytes 
        for (i = 0; i < 0x40; i++)
        {
                n[i] = (unsigned char) (n[i] ^ n[0x40 + i]);
        };

        // step 3 xor bytes 0x0d-0x10 with bytes 0x34-0x38 
        for (i = 0; i < 4; i ++)
        {
                n[0x0d + i] = (unsigned char) (n[0x0d + i] ^ n[0x34 + i]);
        };

        // step 4 xor last 0x40 bytes with first 0x40 bytes 
        for (i = 0; i < 0x40; i ++)
        {
                n[0x40 + i] = (unsigned char) (n[0x40 + i] ^ n[i]);
        };
}

void rsa_modulus(struct Rsa* rsa, unsigned char* dest)
{
        assert(rsa);
        assert(dest);

        const BIGNUM *n = NULL;

        RSA_get0_key(rsa->key, &n, NULL, NULL);
        BN_bn2bin(n, dest);
        rsa_scramble_modulo(dest);
}

int rsa_decrypt(struct Rsa* rsa, unsigned char* src, unsigned char* dest)
{
        assert(rsa);
        assert(src);
        assert(dest);

        return RSA_private_decrypt(
                rsa_size(rsa),
                src,
                dest,
                rsa->key,
                RSA_NO_PADDING
        );
}
