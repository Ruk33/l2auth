#ifndef RSA_H
#define RSA_H

#include <openssl/bn.h>
#include <openssl/rsa.h>

struct Rsa
{
        BIGNUM *e;
        RSA *key;
};

/**
 * Initialize rsa key. This function
 * must be called before using any of the rest.
 */
void rsa_init(struct Rsa *rsa);

/**
 * Free ONLY internal memory
 * requested for BIGNUM *e and
 * RSA *key.
 */
void rsa_free(struct Rsa *rsa);

/**
 * Get rsa key size
 */
int rsa_size(struct Rsa* rsa);

/**
 * Write rsa modulus into dest
 * Dest must be long enough to hold
 * the entire information.
 */
void rsa_modulus(struct Rsa* rsa, unsigned char* dest);

/**
 * Decrypt bytes from src
 * and store the result in dest.
 * The amount of bytes decrypted
 * will be returned.
 */
int rsa_decrypt(struct Rsa* rsa, unsigned char* src, unsigned char* dest);

#endif
