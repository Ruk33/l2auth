#ifndef ACTION_HANDLER_H
#define ACTION_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void action_handler
(struct Client *client, l2_raw_packet *packet);

#endif
