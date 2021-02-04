#include "headers.h"
#include "decrypt_packet.h"

void decrypt_packet(packet *dest, byte_t *src, size_t src_size, byte_t *key)
{
        size_t size_header = 0;
        size_t content_size = 0;

        byte_t *dest_body = NULL;
        byte_t *src_body = NULL;

        int temp = 0;
        int temp2 = 0;
        unsigned int old = 0;

        assert(dest);
        assert(src);
        assert(src_size);
        assert(key);

        size_header = sizeof(short);
        content_size = src_size > 1 ? src_size - size_header : src_size;

        dest_body = dest + size_header;
        src_body = src + size_header;

        for (size_t i = 0; i < content_size; i++) {
               temp2 = src_body[i];
               dest_body[i] = (byte_t) (temp2 ^ key[i & 7] ^ temp);
               temp = temp2;
        }

        old  = (unsigned int) key[0] & 0xff;
        old |= (unsigned int) key[1] << 8 & 0xff00;
        old |= (unsigned int) key[2] << 0x10 & 0xff0000;
        old |= (unsigned int) key[3] << 0x18 & 0xff000000;

        old += (unsigned int) content_size;

        key[0] = (byte_t) (old & 0xff);
        key[1] = (byte_t) (old >> 0x08 &0xff);
        key[2] = (byte_t) (old >> 0x10 &0xff);
        key[3] = (byte_t) (old >> 0x18 &0xff);

        memcpy(dest, (unsigned short *) &src_size, size_header);
}
