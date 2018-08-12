#ifndef L2AUTH_BUILD_RSA_KEY_C
#define L2AUTH_BUILD_RSA_KEY_C

#ifndef LTM_DESC
#define LTM_DESC
#endif

#include <tomcrypt.h>
#include <tommath.h>

struct l2_rsa_key {
  rsa_key key;
};

#define L2_RSA_KEY_DEFAULT_SIZE_IN_BYTES (1024/8)
#define L2_RSA_KEY_DEFAULT_E 65537

void l2_rsa_key_build(struct l2_rsa_key* key, int key_size_in_bytes, long e)
{
    register_prng(&sprng_desc);
    ltc_mp = ltm_desc;
    register_hash(&sha1_desc);
    rsa_make_key(NULL, find_prng("sprng"), key_size_in_bytes, e, &key->key);
}

void l2_rsa_key_modulus(struct l2_rsa_key* key, unsigned char* dest)
{
    mp_to_signed_bin(key->key.N, dest);
}

int l2_rsa_key_size(struct l2_rsa_key* key)
{
    return mp_signed_bin_size(key->key.N);
}

#endif //L2AUTH_BUILD_RSA_KEY_C