#include "include/util.h"
#include "include/packet.h"
#include "include/gs_crypt.h"

void gs_encrypt(byte_t *key, byte_t *dest, packet_t *src)
{
        packet_t *dest_body = 0;
        packet_t *src_body  = 0;

        u16_t src_size  = 0;
        u16_t body_size = 0;

        int temp  = 0;
        int temp2 = 0;

        unsigned int old = 0;

        src_size  = packet_size(src);
        body_size = src_size - 2;
        src_body  = packet_body(src);
        dest_body = packet_body(dest);

        for (u16_t i = 0; i < body_size; i++) {
                temp2        = src_body[i] & 0xff;
                dest_body[i] = (byte_t)(temp2 ^ key[i & 7] ^ temp);
                temp         = dest_body[i];
        }

        old = ((unsigned int) key[0] & 0xff);
        old |= ((unsigned int) key[1] << 8 & 0xff00);
        old |= ((unsigned int) key[2] << 0x10 & 0xff0000);
        old |= ((unsigned int) key[3] << 0x18 & 0xff000000);

        old += (unsigned int) body_size;

        key[0] = (byte_t)(old & 0xff);
        key[1] = (byte_t)(old >> 0x08 & 0xff);
        key[2] = (byte_t)(old >> 0x10 & 0xff);
        key[3] = (byte_t)(old >> 0x18 & 0xff);

        bytes_cpy(dest, (byte_t *) &src_size, 2);
}

void gs_decrypt(byte_t *key, packet_t *dest, byte_t *src)
{
        packet_t *dest_body = 0;
        packet_t *src_body  = 0;

        u16_t body_size = 0;

        int temp  = 0;
        int temp2 = 0;

        unsigned int old = 0;

        body_size = packet_size(src);
        body_size = body_size > 1 ? body_size - 2 : body_size;

        dest_body = packet_body(dest);
        src_body  = packet_body(src);

        for (u16_t i = 0; i < body_size; i++) {
                temp2        = src_body[i];
                dest_body[i] = (byte_t)(temp2 ^ key[i & 7] ^ temp);
                temp         = temp2;
        }

        old = (unsigned int) key[0] & 0xff;
        old |= (unsigned int) key[1] << 8 & 0xff00;
        old |= (unsigned int) key[2] << 0x10 & 0xff0000;
        old |= (unsigned int) key[3] << 0x18 & 0xff000000;

        old += (unsigned int) body_size;

        key[0] = (byte_t)(old & 0xff);
        key[1] = (byte_t)(old >> 0x08 & 0xff);
        key[2] = (byte_t)(old >> 0x10 & 0xff);
        key[3] = (byte_t)(old >> 0x18 & 0xff);

        bytes_cpy(dest, (byte_t *) &body_size, 2);
}
