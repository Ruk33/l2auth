#ifndef D0_HANDLER_H
#define D0_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void d0_handler
(struct Client *client, l2_raw_packet *packet);

#endif
