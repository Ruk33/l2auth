#ifndef L2AUTH_PACKET_CLIENT_DECRYPT_C
#define L2AUTH_PACKET_CLIENT_DECRYPT_C

#include <log/log.h>
#include <core/endian.h>
#include <core/l2_raw_packet.h>
#include <core/l2_blowfish_key.h>
#include <packet/client/decrypt.h>

l2_raw_packet* packet_client_decrypt
(
        struct L2BlowfishKey* blowfish_key,
        unsigned char* data,
        unsigned short data_size
)
{
        unsigned char packet_header_size =
                (unsigned char) sizeof(l2_raw_packet_size);
        unsigned short data_size_without_packet_size_header =
                (unsigned short) (data_size - packet_header_size);
        unsigned char decrypted_content[
                data_size_without_packet_size_header
        ];
        unsigned char *data_without_packet_size_header =
                data + packet_header_size;

        unsigned short decrypted_size = (unsigned short) (
                (data_size_without_packet_size_header / 8 + 1)
                * 8
        );

        if (!data || !data_size) {
                log_fatal("Trying to decrypt null/empty packet");
                return NULL;
        }

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

        return l2_raw_packet_new(
                decrypted_content,
                data_size_without_packet_size_header
        );
}

#endif
