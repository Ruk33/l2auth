#ifndef RESTART_HANDLER_H
#define RESTART_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void restart_handler(struct Client *client, l2_raw_packet *packet);

#endif
