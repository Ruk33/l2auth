#ifndef L2AUTH_SERVER_PACKET_INIT_C
#define L2AUTH_SERVER_PACKET_INIT_C

#include <core/l2_rsa_key.c>
#include <core/l2_packet.c>

void server_packet_init(struct l2_packet* packet, struct l2_rsa_key *rsa_key)
{
    char session_id[] = {
            (char) 0xfd,
            (char) 0x8a,
            (char) 0x22,
            (char) 0x00,
    };

    char protocol[] = {
            (char) 0x5a,
            (char) 0x78,
            (char) 0x00,
            (char) 0x00,
    };

    int rsa_size = l2_rsa_key_size(rsa_key);
    unsigned char rsa_modulus[rsa_size];

    l2_rsa_key_modulus(rsa_key, rsa_modulus);

    packet_init(packet, 0x00);
    packet_write(packet, session_id, sizeof(session_id));
    packet_write(packet, protocol, sizeof(protocol));
    packet_write(packet, rsa_modulus, rsa_size);
}

#endif //L2AUTH_SERVER_PACKET_INIT_C