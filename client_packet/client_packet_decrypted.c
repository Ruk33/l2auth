#ifndef L2AUTH_CLIENT_PACKET_DECRYPTED_C
#define L2AUTH_CLIENT_PACKET_DECRYPTED_C

#include <core/l2_packet.c>
#include <core/l2_blowfish_key.c>
#include <core/decrypt_client_request.c>

l2_raw_packet* client_packet_decrypted(
        struct l2_blowfish_key* blowfish_key,
        unsigned char* data,
        unsigned short data_size
)
{
        unsigned char packet_length_header_size = sizeof(unsigned short);
        unsigned short data_size_without_packet_size_header = data_size - packet_length_header_size;
        unsigned char decrypted_content[data_size_without_packet_size_header];
        unsigned char *data_without_packet_size_header = data + packet_length_header_size;

        if (!data || !data_size)
                return NULL;

        decrypt_client_request(
                blowfish_key,
                data_without_packet_size_header,
                data_size_without_packet_size_header,
                decrypted_content
        );

        return l2_raw_packet_new(
                decrypted_content,
                data_size_without_packet_size_header
        );
}

#endif //L2AUTH_CLIENT_PACKET_DECRYPTED_C
