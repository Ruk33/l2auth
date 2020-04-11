#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <game/packet/enter_world.h>
#include <game/handler/encrypt.h>
#include <game/handler/enter_world.h>

void game_handler_enter_world
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

        l2_packet* response = game_packet_enter_world(client);
        log_info("Entering world!");
        
        l2_client_send_packet(
                client,
                game_handler_encrypt(response, encrypt_key)
        );
}
