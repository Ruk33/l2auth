#ifndef INCLUDE_RSA_H
#define INCLUDE_RSA_H

#include "util.h"

typedef void rsa_t;

rsa_t *rsa_new();

size_t rsa_size(rsa_t *rsa);

void rsa_modulus(rsa_t *rsa, byte_t *dest);

int rsa_decrypt(rsa_t *rsa, byte_t *dest, byte_t *src);

#endif
