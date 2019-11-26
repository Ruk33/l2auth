#ifndef L2AUTH_BLOWFISH_KEY_C
#define L2AUTH_BLOWFISH_KEY_C

#include <string.h>
#include <openssl/blowfish.h>
#include <log/log.h>

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
        if (!key) {
                log_fatal("Trying to build blowfish key but no key was passed");
                return;
        }
        if (!raw_key || !raw_key_size) {
                log_fatal("Trying to create blowfish key but no raw key was passed");
                return;
        }

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
        if (!key) {
                log_fatal("Trying to encrypt blowfish but no key was passed");
                return;
        }
        if (!src) {
                log_fatal("Trying to encrypt blowfish but no source was passed");
                return;
        }
        if (!dest) {
                log_fatal("Trying to encrypt blowfish but no destination was passed");
                return;
        }
        BF_ecb_encrypt(src, dest, &key->key, BF_ENCRYPT);
}

void l2_blowfish_decrypt
(
        struct L2BlowfishKey* key,
        unsigned char* src,
        unsigned char* dest
)
{
        if (!key) {
                log_fatal("Trying to decrypt blowfish but no key was passed");
                return;
        }
        if (!src) {
                log_fatal("Trying to decrypt blowfish but no source was passed");
                return;
        }
        if (!dest) {
                log_fatal("Trying to decrypt blowfish but no destination was passed");
                return;
        }
        BF_ecb_encrypt(src, dest, &key->key, BF_DECRYPT);
}

#endif
