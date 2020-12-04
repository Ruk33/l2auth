#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <openssl/blowfish.h>
#include "blowfish.h"

void blowfish_init(struct Blowfish *blowfish)
{
        assert(blowfish);
        unsigned char raw_key[] = "_;5.]94-31==-%xT!^[$";
        BF_set_key(&blowfish->key, (int) sizeof(raw_key), raw_key);
}

static uint32_t decode32le(unsigned char *buf)
{
        assert(buf);
        return (uint32_t) buf[0] | ((uint32_t) buf[1] << 8) | ((uint32_t) buf[2] << 16) | ((uint32_t) buf[3] << 24);
}

static uint32_t decode32be(unsigned char *buf)
{
        assert(buf);
        return (uint32_t) buf[3] | ((uint32_t) buf[2] << 8) | ((uint32_t) buf[1] << 16) | ((uint32_t) buf[0] << 24);
}

static void encode32le(unsigned char* buf, uint32_t val)
{
        assert(buf);
        buf[0] = (unsigned char) val;
        buf[1] = (unsigned char) (val >> 8);
        buf[2] = (unsigned char) (val >> 16);
        buf[3] = (unsigned char) (val >> 24);
}

static void encode32be(unsigned char* buf, uint32_t val)
{
        assert(buf);
        buf[3] = (unsigned char) val;
        buf[2] = (unsigned char) (val >> 8);
        buf[1] = (unsigned char) (val >> 16);
        buf[0] = (unsigned char) (val >> 24);
}

size_t blowfish_encrypt(struct Blowfish *blowfish, unsigned char *dest, unsigned char *src, size_t src_size)
{
        assert(blowfish);
        assert(src);
        assert(dest);

        size_t dest_size = (src_size + 7) & (~7);

        for (size_t i = 0; i < dest_size; i += 8)
        {
                // Blowfish uses big endian
                encode32be(dest + i, decode32le(src + i));
                encode32be(dest + i + 4, decode32le(src + i + 4));

                BF_ecb_encrypt(
                        (dest + i),
                        (dest + i),
                        &blowfish->key,
                        BF_ENCRYPT
                );

                // Enforce little endian since that's
                // the endianness used by packets
                // sent to the Lineage 2 client
                encode32le(dest + i, decode32be(dest + i));
                encode32le(dest + i + 4, decode32be(dest + i + 4));
        }

        return dest_size;
}


void blowfish_decrypt(struct Blowfish *blowfish, unsigned char *dest, unsigned char *src, size_t src_size)
{
        assert(blowfish);
        assert(dest);
        assert(src);

        for (size_t i = 0, max = (src_size + 7) & (~7); i < max; i += 8)
        {
                // Blowfish uses big endian
                encode32be(dest + i, decode32le(src + i));
                encode32be(dest + i + 4, decode32le(src + i + 4));

                BF_ecb_encrypt(
                        (dest + i),
                        (dest + i),
                        &blowfish->key,
                        BF_DECRYPT
                );

                // Turn endianness back to the host machine
                encode32le(dest + i, decode32be(dest + i));
                encode32le(dest + i + 4, decode32be(dest + i + 4));
        }
}
