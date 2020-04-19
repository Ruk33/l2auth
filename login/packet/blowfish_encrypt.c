#include <assert.h>
#include <stdlib.h>
#include <core/endian.h>
#include <core/l2_client.h>
#include <core/l2_blowfish_key.h>
#include <core/l2_raw_packet.h>
#include <login/packet/blowfish_encrypt.h>

l2_raw_packet* login_packet_blowfish_encrypt
(
        struct L2Client* client,
        l2_raw_packet* to_encrypt
)
{
        assert(client);
        assert(to_encrypt);

        struct L2BlowfishKey* key = l2_client_blowfish_key(client);

        unsigned short packet_size = l2_raw_packet_get_size(to_encrypt);
        unsigned char* packet_content = l2_client_alloc_temp_mem(
                client,
                packet_size * sizeof(char)
        );

        unsigned short encrypted_packet_size = (unsigned short) (
                (packet_size / 8 + 1) * 8
        );
        unsigned char* encrypted_content = l2_client_alloc_temp_mem(
                client,
                encrypted_packet_size * sizeof(char)
        );

        l2_raw_packet* raw_packet = l2_client_alloc_temp_mem(
                client,
                l2_raw_packet_calculate_size(encrypted_packet_size)
        );

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
                /*
                 * (franco.montenegro)
                 * Blowfish uses big endian, Lineage 2 send
                 * us little endian, convert little to big
                 */
                encode32be(
                        encrypted_content + i,
                        decode32le(packet_content + i)
                );
                encode32be(
                        encrypted_content + i + 4,
                        decode32le(packet_content + i + 4)
                );

                l2_blowfish_encrypt(
                        key,
                        encrypted_content + i,
                        encrypted_content + i
                );

                // Convert it back from big endian to little endian
                encode32le(
                        encrypted_content + i,
                        decode32be(encrypted_content + i)
                );
                encode32le(
                        encrypted_content + 4 + i,
                        decode32be(encrypted_content + 4 + i)
                );
        }

        l2_raw_packet_init(
                raw_packet,
                encrypted_content,
                encrypted_packet_size
        );

        return raw_packet;
}
