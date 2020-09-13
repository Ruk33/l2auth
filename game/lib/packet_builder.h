#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H

#include <stdlib.h>
#include <core/l2_packet.h>
#include "client.h"

l2_packet *packet_builder_new
(
        struct Client *client,
        l2_packet_type type,
        unsigned char *buf,
        size_t buf_size
);

#endif
