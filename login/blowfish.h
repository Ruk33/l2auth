#ifndef BLOWFISH_H
#define BLOWFISH_H

#include <stdlib.h>
#include <openssl/blowfish.h>

struct Blowfish {
        BF_KEY key;
};

/**
 * This function must be called
 * before using any of the rest.
 */
void blowfish_init(struct Blowfish *blowfish);

/**
 * Encrypt bytes from src and
 * store the result in dest.
 * dest must be long enough to hold
 * the entire result. Return dest size
 * after being encrypted.
 */
size_t blowfish_encrypt(struct Blowfish *blowfish, unsigned char *dest, unsigned char *src, size_t src_size);

/**
 * Decrypt bytes from src and
 * store the result in dest.
 * dest must be long enough to hold
 * the entire result.
 */
void blowfish_decrypt(struct Blowfish *blowfish, unsigned char *dest, unsigned char *src, size_t src_size);

#endif
