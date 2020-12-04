#include <assert.h>
#include "../byte_write.h"
#include "../packet.h"
#include "play_ok.h"

void server_packet_play_ok(unsigned char *dest, int playOK1, int playOK2)
{
        assert(dest);
        assert(playOK1 > 0);
        assert(playOK2 > 0);

        unsigned char type = 0x07;
        unsigned char buf[SERVER_PACKET_PLAY_OK_SIZE] = {0};
        unsigned char *p = buf;

        BYTE_WRITE_VAL(p, playOK1);
        BYTE_WRITE_VAL(p, playOK2);

        packet_build(dest, type, buf, p - buf);
}
