#ifndef REQUEST_H
#define REQUEST_H

#include "../../../include/util.h"
#include "state.h"

void handle_request(struct state *state,
                    struct client *client,
                    struct buffer *req);

#endif