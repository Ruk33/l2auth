#include "headers.h"
#include "encrypt_packet.h"

void encrypt_packet(byte_t *dest, packet *src, size_t src_size, byte_t *key)
{
        size_t size_header = 0;

        byte_t *packet_content = 0;
        byte_t *dest_content = 0;

        int temp = 0;
        int temp2 = 0;
        unsigned int old = 0;

        assert(dest);
        assert(src);
        assert(src_size);
        assert(key);

        size_header = sizeof(short);
        packet_content = src + size_header;
        dest_content = dest + size_header;

        for (size_t i = 0; i < src_size - size_header; i++) {
               temp2 = packet_content[i] & 0xff;
               dest_content[i] = (byte_t) (temp2 ^ key[i & 7] ^ temp);
               temp = dest_content[i];
        }

        old  = ((unsigned int) key[0] &0xff);
        old |= ((unsigned int) key[1] << 8 &0xff00);
        old |= ((unsigned int) key[2] << 0x10 &0xff0000);
        old |= ((unsigned int) key[3] << 0x18 &0xff000000);

        old += (unsigned short) (src_size - size_header);

        key[0] = (byte_t) (old &0xff);
        key[1] = (byte_t) (old >> 0x08 &0xff);
        key[2] = (byte_t) (old >> 0x10 &0xff);
        key[3] = (byte_t) (old >> 0x18 &0xff);

        memcpy(dest, (unsigned short *) &src_size, size_header);
}
