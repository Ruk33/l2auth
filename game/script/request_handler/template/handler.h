#ifndef PACKET_NAME_HANDLER_H
#define PACKET_NAME_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void PACKET_LOWERCASE_NAME_handler
(struct Client *client, l2_raw_packet *packet);

#endif
