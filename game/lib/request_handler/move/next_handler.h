#ifndef MOVE_NEXT_HANDLER_H
#define MOVE_NEXT_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void move_next_handler
(struct Client *client, l2_raw_packet *packet);

#endif
