#include <openssl/blowfish.h>
#include "include/config.h"
#include "include/util.h"
#include "include/blowfish.h"

static BF_KEY keys[MAX_CLIENTS] = { 0 };
static size_t key_count         = 0;

blowfish_t *blowfish_new(void)
{
        byte_t raw_key[] = "_;5.]94-31==-%xT!^[$";

        BF_set_key(&keys[key_count], (int) sizeof(raw_key), raw_key);
        key_count += 1;

        return (blowfish_t *) &keys[key_count - 1];
}

void blowfish_encrypt(blowfish_t *key, byte_t *dest, byte_t *src, size_t n)
{
        for (size_t i = 0, iters = (n + 7) & (~7); i < iters; i += 8) {
                // Blowfish uses big endian
                // Todo: Convert based on current platform.
                encode32be(dest + i, decode32le(src + i));
                encode32be(dest + i + 4, decode32le(src + i + 4));

                BF_ecb_encrypt((dest + i), (dest + i), key, BF_ENCRYPT);

                // Back to little endian (endianess used by Lineage 2)
                encode32le(dest + i, decode32be(dest + i));
                encode32le(dest + i + 4, decode32be(dest + i + 4));
        }
}

void blowfish_decrypt(blowfish_t *key, byte_t *dest, byte_t *src, size_t n)
{
        for (size_t i = 0, iters = (n + 7) & (~7); i < iters; i += 8) {
                // Blowfish uses big endian
                // Todo: Convert based on current platform.
                encode32be(dest + i, decode32le(src + i));
                encode32be(dest + i + 4, decode32le(src + i + 4));

                BF_ecb_encrypt((dest + i), (dest + i), key, BF_DECRYPT);

                // Back to little endian (endianess used by Lineage 2)
                encode32le(dest + i, decode32be(dest + i));
                encode32le(dest + i + 4, decode32be(dest + i + 4));
        }
}
