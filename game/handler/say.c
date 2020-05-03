#include <assert.h>
#include <stdlib.h>
#include <core/l2_raw_packet.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_string.h>
#include <game/packet/creature_say.h>
#include <game/handler/say.h>

void game_handler_say
(
        struct L2Server* server,
        struct L2Client* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        assert(server);
        assert(client);
        assert(request);
        assert(encrypt_key);

        l2_string* message = (l2_string *) (request + 3);
        l2_packet* response = game_packet_creature_say(client, message);

        l2_server_broadcast_packet_to_clients(
                server,
                response
        );
}
