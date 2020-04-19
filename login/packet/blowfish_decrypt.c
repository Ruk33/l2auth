#include <assert.h>
#include <core/endian.h>
#include <core/l2_client.h>
#include <core/l2_raw_packet.h>
#include <core/l2_blowfish_key.h>
#include <login/packet/blowfish_decrypt.h>

l2_raw_packet* login_packet_blowfish_decrypt
(
        struct L2Client* client,
        unsigned char* data,
        unsigned short data_size
)
{
        assert(client);
        assert(data);

        struct L2BlowfishKey* blowfish_key =
                l2_client_blowfish_key(client);

        unsigned char packet_header_size =
                (unsigned char) sizeof(l2_raw_packet_size);

        unsigned short data_size_without_packet_size_header =
                (unsigned short) (data_size - packet_header_size);

        unsigned char* decrypted_content = l2_client_alloc_temp_mem(
                client,
                data_size_without_packet_size_header
        );

        unsigned char* data_without_packet_size_header =
                data + packet_header_size;

        unsigned short decrypted_size = (unsigned short) (
                (data_size_without_packet_size_header / 8 + 1)
                * 8
        );

        for (
                unsigned short i = 0;
                i < decrypted_size;
                i = (unsigned short) (i + 8)
        ) {
                // blowfish uses big endian, l2 sends us little endian
                // convert little endian to big endian
                encode32be(
                        decrypted_content + i,
                        decode32le(
                                data_without_packet_size_header
                                + i
                        )
                );
                encode32be(
                        decrypted_content + i + 4,
                        decode32le(
                                data_without_packet_size_header
                                + i
                                + 4
                        )
                );

                l2_blowfish_decrypt(
                        blowfish_key,
                        decrypted_content + i,
                        decrypted_content + i
                );

                // convert it back from big endian to little endian
                encode32le(
                        decrypted_content + i,
                        decode32be(decrypted_content + i)
                );
                encode32le(
                        decrypted_content + i + 4,
                        decode32be(decrypted_content + i + 4)
                );
        }

        return l2_client_create_raw_packet(
                client,
                decrypted_content,
                data_size_without_packet_size_header
        );
}
