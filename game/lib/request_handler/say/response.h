#ifndef SAY_RESPONSE_H
#define SAY_RESPONSE_H

#include <core/l2_string.h>
#include <core/l2_packet.h>
#include "../../client.h"

l2_packet *say_response
(struct Client *client, l2_string* message);

#endif
