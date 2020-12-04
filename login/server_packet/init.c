#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../byte_write.h"
#include "../packet.h"
#include "../rsa.h"
#include "init.h"

void server_packet_init(unsigned char *dest, struct Rsa *rsa)
{
        assert(dest);
        assert(rsa);

        unsigned char type = 0x00;
        struct ServerPacketInit packet = {0};

        unsigned char session_id[] = {
                0xfd,
                0x8a,
                0x22,
                0x00,
        };

        unsigned char protocol[] = {
                0x5a,
                0x78,
                0x00,
                0x00,
        };

        int key_size = rsa_size(rsa);

        unsigned char buf[SERVER_PACKET_INIT_SIZE] = {0};
        unsigned char *p = buf;

        memcpy(packet.session_id, session_id, sizeof(packet.session_id));
        memcpy(packet.protocol, protocol, sizeof(packet.protocol));
        rsa_modulus(rsa, packet.modulus);

        BYTE_WRITE_VAL(p, session_id);
        BYTE_WRITE_VAL(p, protocol);
        BYTE_WRITE(p, packet.modulus, key_size);

        packet_build(dest, type, buf, p - buf);
}
