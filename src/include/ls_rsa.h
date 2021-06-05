#ifndef INCLUDE_LS_RSA_H
#define INCLUDE_LS_RSA_H

#include "util.h"

typedef void ls_rsa_t;

ls_rsa_t *ls_rsa_new();

size_t ls_rsa_size(ls_rsa_t *rsa);

void ls_rsa_modulus(ls_rsa_t *rsa, byte_t *dest);

int ls_rsa_decrypt(ls_rsa_t *rsa, byte_t *dest, byte_t *src);

#endif
