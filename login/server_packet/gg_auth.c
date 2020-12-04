#include <assert.h>
#include "../packet.h"
#include "gg_auth.h"

void server_packet_gg_auth(unsigned char *dest, enum ServerPacketGGAuthResponse response)
{
        assert(dest);
        unsigned char type = 0x0b;
        packet_build(dest, type, (unsigned char *) &response, sizeof(response));
}
