#ifndef L2AUTH_BLOWFISH_KEY_C
#define L2AUTH_BLOWFISH_KEY_C

#include <string.h>
#include <openssl/blowfish.h>

#define BLOWFISH_MAX_KEY_SIZE_LENGTH 56

struct L2BlowfishKey
{
        unsigned char raw_key[BLOWFISH_MAX_KEY_SIZE_LENGTH];
        BF_KEY key;
};

void l2_blowfish_key_build
(
        struct L2BlowfishKey* key,
        unsigned char* raw_key,
        int raw_key_size
)
{
        memcpy(key->raw_key, raw_key, (size_t) raw_key_size);
        BF_set_key(&key->key, raw_key_size, key->raw_key);
}

void l2_blowfish_encrypt
(
        struct L2BlowfishKey* key,
        unsigned char* src,
        unsigned char* dest
)
{
        BF_ecb_encrypt(src, dest, &key->key, BF_ENCRYPT);
}

void l2_blowfish_decrypt
(
        struct L2BlowfishKey* key,
        unsigned char* src,
        unsigned char* dest
)
{
        BF_ecb_encrypt(src, dest, &key->key, BF_DECRYPT);
}

#endif
