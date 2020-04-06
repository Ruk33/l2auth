#ifndef L2AUTH_BLOWFISH_KEY_C
#define L2AUTH_BLOWFISH_KEY_C

#include <stdlib.h>
#include <string.h>
#include <openssl/blowfish.h>
#include <log/log.h>
#include <core/l2_blowfish_key.h>

struct L2BlowfishKey* l2_blowfish_key_new()
{
        char raw_key[] = "_;5.]94-31==-%xT!^[$";
        size_t raw_key_size = sizeof(raw_key);
        struct L2BlowfishKey* l2_key = calloc(1, sizeof(struct L2BlowfishKey));

        l2_key->raw_key = calloc(raw_key_size, sizeof(char));
        memcpy(l2_key->raw_key, raw_key, raw_key_size);

        l2_key->key = calloc(1, sizeof(BF_KEY));
        BF_set_key(l2_key->key, (int) raw_key_size, l2_key->raw_key);

        return l2_key;
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
        BF_ecb_encrypt(src, dest, key->key, BF_ENCRYPT);
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
        BF_ecb_encrypt(src, dest, key->key, BF_DECRYPT);
}

#endif
