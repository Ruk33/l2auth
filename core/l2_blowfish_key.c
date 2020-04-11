#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/blowfish.h>
#include <log/log.h>
#include <core/l2_blowfish_key.h>

struct L2BlowfishKey
{
        unsigned char raw_key[21];
        BF_KEY key;
};

size_t l2_blowfish_key_struct_size()
{
        return sizeof(struct L2BlowfishKey);
}

void l2_blowfish_key_init(struct L2BlowfishKey* l2_key)
{
        assert(l2_key);

        char raw_key[] = "_;5.]94-31==-%xT!^[$";
        size_t raw_key_size = sizeof(raw_key);

        memcpy(l2_key->raw_key, raw_key, raw_key_size);
        BF_set_key(&l2_key->key, (int) raw_key_size, l2_key->raw_key);
}

void l2_blowfish_encrypt
(
        struct L2BlowfishKey* l2_key,
        unsigned char* src,
        unsigned char* dest
)
{
        assert(l2_key);
        assert(src);
        assert(dest);
        BF_ecb_encrypt(src, dest, &l2_key->key, BF_ENCRYPT);
}

void l2_blowfish_decrypt
(
        struct L2BlowfishKey* l2_key,
        unsigned char* src,
        unsigned char* dest
)
{
        assert(l2_key);
        assert(src);
        assert(dest);
        BF_ecb_encrypt(src, dest, &l2_key->key, BF_DECRYPT);
}
