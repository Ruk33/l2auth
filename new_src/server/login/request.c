#include <assert.h>
#include "../../include/packet.h"
#include "include/request.h"

void handle_request(struct state *state, struct client *client)
{
    assert(state);
    assert(client);

    // Make sure the entire packet has arrived.
    if (packet_size(&client->request.packet) != client->request.received) {
        client->request.is_partial = 1;
        return;
    }

    
}