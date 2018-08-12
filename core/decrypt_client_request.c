#ifndef L2AUTH_DECRYPT_CLIENT_REQUEST_C
#define L2AUTH_DECRYPT_CLIENT_REQUEST_C

#include <core/endian.c>
#include <core/l2_blowfish_key.c>

void decrypt_client_request(struct l2_blowfish_key* key, char* src, int src_size, unsigned char* dest)
{
    for (int i = 0, max = src_size/8; i<=max; i++) {
        // blowfish uses big endian, l2 sends us little endian
        // convert little endian to big endian
        encode32be(dest+0+i*8, decode32le(src+0+i*8));
        encode32be(dest+4+i*8, decode32le(src+4+i*8));

        l2_blowfish_decrypt(key, dest+i*8, dest+i*8);

        // convert it back from big endian to little endian
        encode32le(dest+i*8, decode32be(dest+i*8));
        encode32le(dest+4+i*8, decode32be(dest+4+i*8));
    }
}

#endif //L2AUTH_DECRYPT_CLIENT_REQUEST_C