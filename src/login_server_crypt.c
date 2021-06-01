#include "include/util.h"
#include "include/blowfish.h"
#include "include/packet.h"
#include "include/login_server_crypt.h"

void login_server_encrypt(blowfish_t *k, byte_t *dest, packet_t *src)
{
        u16_t body_size      = 0;
        u16_t encrypted_size = 0;

        body_size      = packet_size(src) - 2;
        encrypted_size = ((body_size + 7) & (~7)) + 2;

        bytes_cpy(dest, (byte_t *) &encrypted_size, 2);
        blowfish_encrypt(k, dest + 2, src + 2, body_size);
}

void login_server_decrypt(blowfish_t *k, packet_t *dest, byte_t *src)
{
        u16_t src_size = 0;

        src_size = packet_size(src);

        bytes_cpy(dest, src, 2);
        blowfish_decrypt(k, dest + 2, src + 2, src_size - 2);
}
