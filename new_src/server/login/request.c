#include <assert.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "include/state.h"
#include "include/request.h"

void handle_request(struct state *state,
                    struct client *client,
                    struct buffer *req)
{
    assert(state);
    assert(client);
    assert(req);
    assert(req->buf);

    // Copy raw bytes from client to client->request.

    // Make sure the entire packet has arrived.
    if (packet_size(&client->request.packet) != req->used) {
        client->request.is_partial = 1;
        return;
    }

    
}