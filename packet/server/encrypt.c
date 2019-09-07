#ifndef L2AUTH_PACKET_SERVER_ENCRYPT_C
#define L2AUTH_PACKET_SERVER_ENCRYPT_C

#include <core/l2_blowfish_key.c>
#include <core/l2_raw_packet.c>
#include <core/endian.c>

l2_raw_packet* packet_server_encrypt
(
        l2_raw_packet* to_encrypt,
        struct l2_blowfish_key* key
)
{
        unsigned short packet_size = l2_raw_packet_get_size(
                to_encrypt
        );
        unsigned char packet_content[packet_size];

        unsigned short encrypted_packet_size = 
                (unsigned short) ((packet_size / 8 + 1) * 8);
        unsigned char encrypted_content[encrypted_packet_size];

        l2_raw_packet_content(
                to_encrypt,
                packet_content,
                0,
                packet_size
        );

        for (
                unsigned short i = 0;
                i < encrypted_packet_size;
                i = (unsigned short) (i + 8)
        ) {
                // blowfish uses big endian, l2 sends us little endian
                // convert little endian to big endian
                encode32be(encrypted_content + i, decode32le(packet_content + i));
                encode32be(encrypted_content + i + 4, decode32le(packet_content + i + 4));

                l2_blowfish_encrypt(key, encrypted_content + i, encrypted_content + i);

                // convert it back from big endian to little endian
                encode32le(encrypted_content + i, decode32be(encrypted_content + i));
                encode32le(encrypted_content + 4 + i, decode32be(encrypted_content + 4 + i));
        }

        return l2_raw_packet_new(
                encrypted_content,
                encrypted_packet_size
        );
}

#endif
