#include <assert.h>
#include <core/l2_string.h>
#include <game/request.h>
#include <game/server.h>
#include <game/client.h>
#include "next_handler.h"
#include "response.h"
#include "handler.h"

void game_request_say_handler(struct GameRequest* request)
{
        assert(request);

        struct GameClient* server = request->conn->server;
        struct GameClient* client = request->conn->client;
        l2_string* message = (l2_string *) l2_packet_content(request->packet);
        l2_packet* response = game_request_say_response(client, message);

        game_server_broadcast_packet_to_clients(
                server,
                response
        );

        request->conn->handler = game_request_say_next_handler;
}
