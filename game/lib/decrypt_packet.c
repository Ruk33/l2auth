#include "headers.h"
#include "decrypt_packet.h"

void decrypt_packet(byte_t *dest, packet *src, size_t src_size, byte_t *key)
{
        assert(dest);
        assert(src);
        assert(src_size);
        assert(key);

        /*
         * TODO
         * This is temporary and needs to be changed.
         * We can't use this much stack memory.
         */
        byte_t decrypted[65536] = {0};

        size_t size_header = sizeof(short);
        size_t type_header = sizeof(char);
        size_t packet_final_size = (
                src_size > 1 ? src_size - size_header : src_size
        );

        byte_t *packet_content = src + size_header;

        int temp = 0;
        int temp2 = 0;
        unsigned int old = 0;

        for (size_t i = 0; i < packet_final_size; i++)
        {
               temp2 = packet_content[i];
               decrypted[i] = (byte_t) (temp2 ^ key[i & 7] ^ temp);
               temp = temp2;
        };

        old  = (unsigned int) key[0] & 0xff;
        old |= (unsigned int) key[1] << 8 & 0xff00;
        old |= (unsigned int) key[2] << 0x10 & 0xff0000;
        old |= (unsigned int) key[3] << 0x18 & 0xff000000;

        old += (unsigned int) (packet_final_size);

        key[0] = (byte_t) (old & 0xff);
        key[1] = (byte_t) (old >> 0x08 &0xff);
        key[2] = (byte_t) (old >> 0x10 &0xff);
        key[3] = (byte_t) (old >> 0x18 &0xff);

        packet_build(
                dest,
                decrypted[0],
                decrypted + type_header,
                packet_final_size
        );
}
