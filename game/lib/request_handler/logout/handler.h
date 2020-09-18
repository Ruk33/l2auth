#ifndef LOGOUT_HANDLER_H
#define LOGOUT_HANDLER_H

#include <core/l2_raw_packet.h>
#include "../../client.h"

void logout_handler
(struct Client *client, l2_raw_packet *packet);

#endif
