#ifndef MOVE_HANDLER_H
#define MOVE_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void move_handler
(struct Client *client, l2_raw_packet *packet);

#endif
