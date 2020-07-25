#ifndef PROTOCOL_VERSION_NEXT_HANDLER_H
#define PROTOCOL_VERSION_NEXT_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void protocol_version_next_handler(struct Client *client, l2_raw_packet *packet);

#endif
