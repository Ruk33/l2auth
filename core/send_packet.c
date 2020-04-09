#ifndef L2AUTH_SEND_PACKET_C
#define L2AUTH_SEND_PACKET_C

#include <assert.h>
#include <core/l2_socket.h>
#include <core/l2_packet.h>
#include <core/send_packet.h>

void send_packet
(
        struct L2Socket* client,
        l2_raw_packet* packet
)
{
        assert(client);
        assert(packet);
        l2_socket_send(
                client,
                packet,
                l2_raw_packet_get_size(packet)
        );
}

#endif
