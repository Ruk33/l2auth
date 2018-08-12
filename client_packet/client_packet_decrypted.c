#ifndef L2AUTH_CLIENT_PACKET_DECRYPTED_C
#define L2AUTH_CLIENT_PACKET_DECRYPTED_C

#include <core/l2_packet.c>
#include <core/l2_blowfish_key.c>
#include <core/decrypt_client_request.c>

void client_packet_decrypted(
        struct l2_packet* packet,
        struct l2_blowfish_key* blowfish_key,
        char* raw_data,
        int raw_data_size
)
{
    char *raw_data_without_packet_size_header = raw_data+2;
    int raw_data_size_without_packet_size_header = raw_data_size-2;

    packet_reset(packet);
    decrypt_client_request(
            blowfish_key,
            raw_data_without_packet_size_header,
            raw_data_size_without_packet_size_header,
            packet->buffer
    );
}

#endif //L2AUTH_CLIENT_PACKET_DECRYPTED_C