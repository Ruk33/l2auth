#ifndef VALIDATE_POSITION_NEXT_HANDLER_H
#define VALIDATE_POSITION_NEXT_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void validate_position_next_handler(struct Client *client, l2_raw_packet *packet);

#endif
