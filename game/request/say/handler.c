#include <assert.h>
#include <stdlib.h>
#include <core/l2_raw_packet.h>
#include <core/l2_string.h>
#include <game/server.h>
#include <game/client.h>
#include "response.h"
#include "handler.h"

void game_request_say_handler
(
        struct GameServer* server,
        struct GameClient* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        assert(server);
        assert(client);
        assert(request);
        assert(encrypt_key);

        l2_string* message = (l2_string *) (request + 3);
        l2_packet* response = game_request_say_response(client, message);

        game_server_broadcast_packet_to_clients(
                server,
                response
        );
}
