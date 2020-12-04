#include <assert.h>
#include <string.h>
#include "../byte_write.h"
#include "../packet.h"
#include "ok.h"

void server_packet_ok(unsigned char *dest, int loginOK1, int loginOK2)
{
        assert(dest);
        assert(loginOK1 > 0);
        assert(loginOK2 > 0);

        unsigned char type = 0x03;
        struct ServerPacketOk packet = {0};
        unsigned char after_key[] = {
                0x00,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0xea,
                0x03,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x02,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x60,
                0x62,
                0xe0,
                0x00,

                0x00,
                0x00,
                0x00,
        };

        unsigned char buf[SERVER_PACKET_OK_FULL_SIZE] = {0};
        unsigned char *p = buf;

        packet.loginOK1 = loginOK1;
        packet.loginOK2 = loginOK2;
        memcpy(packet.after_key, after_key, sizeof(packet.after_key));

        BYTE_WRITE_VAL(p, packet.loginOK1);
        BYTE_WRITE_VAL(p, packet.loginOK2);
        BYTE_WRITE(p, packet.after_key, sizeof(packet.after_key));

        packet_build(dest, type, buf, p - buf);
}
