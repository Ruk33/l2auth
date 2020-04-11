#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <game/packet/restart.h>
#include <game/handler/encrypt.h>
#include <game/handler/auth_login.h>
#include <game/handler/restart.h>

void game_handler_restart
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

        l2_packet* response = game_packet_restart(client);
        log_info("Restarting");
        
        l2_client_send_packet(
                client,
                game_handler_encrypt(response, encrypt_key)
        );

        game_handler_auth_login(
                server,
                client,
                request,
                encrypt_key
        );
}
