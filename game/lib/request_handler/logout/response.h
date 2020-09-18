#ifndef LOGOUT_RESPONSE_H
#define LOGOUT_RESPONSE_H

#include <core/l2_packet.h>
#include "../../client.h"

l2_packet *logout_response
(struct Client *client);

#endif
