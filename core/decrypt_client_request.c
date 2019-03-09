#ifndef L2AUTH_DECRYPT_CLIENT_REQUEST_C
#define L2AUTH_DECRYPT_CLIENT_REQUEST_C

#include <core/endian.c>
#include <core/l2_blowfish_key.c>

void decrypt_client_request(
        struct l2_blowfish_key* key,
        unsigned char* src,
        unsigned short src_size,
        unsigned char* dest
)
{
    for (unsigned short i = 0, max = (src_size / 8 + 1) * 8; i < max; i += 8) {
        // blowfish uses big endian, l2 sends us little endian
        // convert little endian to big endian
        encode32be(dest + i, decode32le(src + i));
        encode32be(dest + i + 4, decode32le(src + i + 4));

        l2_blowfish_decrypt(key, dest + i, dest + i);

        // convert it back from big endian to little endian
        encode32le(dest + i, decode32be(dest + i));
        encode32le(dest + i + 4, decode32be(dest + i + 4));
    }
}

#endif //L2AUTH_DECRYPT_CLIENT_REQUEST_C
