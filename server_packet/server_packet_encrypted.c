#ifndef L2AUTH_SERVER_PACKET_ENCRYPTED_C
#define L2AUTH_SERVER_PACKET_ENCRYPTED_C

#include <core/l2_blowfish_key.c>
#include <core/l2_packet.c>
#include <core/endian.c>

void server_packet_encrypted(struct l2_packet* packet, struct l2_packet* to_encrypt, struct l2_blowfish_key* key)
{
    for (int i = 0, max = to_encrypt->size/8; i<=max; i++) {
        // blowfish uses big endian, l2 sends us little endian
        // convert little endian to big endian
        encode32be(packet->buffer+0+i*8, decode32le(to_encrypt->buffer+0+i*8));
        encode32be(packet->buffer+4+i*8, decode32le(to_encrypt->buffer+4+i*8));

        l2_blowfish_encrypt(key, packet->buffer+i*8, (unsigned char*) (packet->buffer+i*8));

        // convert it back from big endian to little endian
        encode32le(packet->buffer+i*8, decode32be(packet->buffer+i*8));
        encode32le(packet->buffer+4+i*8, decode32be(packet->buffer+4+i*8));

        packet->size = (unsigned short) ((i+1)*8);
    }
}

#endif //L2AUTH_SERVER_PACKET_ENCRYPTED_C