#ifndef INCLUDE_BLOWFISH_H
#define INCLUDE_BLOWFISH_H

#include "util.h"

typedef void blowfish_t;

blowfish_t *blowfish_new(void);

// Encrypt n bytes from src to dest using key.
void blowfish_encrypt(blowfish_t *key, byte_t *dest, byte_t *src, size_t n);

// Decrypt n bytes from src to dest using key.
void blowfish_decrypt(blowfish_t *key, byte_t *dest, byte_t *src, size_t n);

#endif
