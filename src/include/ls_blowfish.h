#ifndef INCLUDE_LS_BLOWFISH_H
#define INCLUDE_LS_BLOWFISH_H

#include "util.h"

typedef void ls_blowfish_t;

ls_blowfish_t *ls_blowfish_new(void);

// Encrypt n bytes from s to d using key.
void ls_blowfish_encrypt(ls_blowfish_t *key, byte_t *d, byte_t *s, size_t n);

// Decrypt n bytes from s to d using key.
void ls_blowfish_decrypt(ls_blowfish_t *key, byte_t *d, byte_t *s, size_t n);

#endif
