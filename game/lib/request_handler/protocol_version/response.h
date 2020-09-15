#ifndef PROTOCOL_VERSION_RESPONSE_H
#define PROTOCOL_VERSION_RESPONSE_H

#include <core/l2_packet.h>
#include "../../client.h"

l2_packet *protocol_version_response
(struct Client *client);

#endif
