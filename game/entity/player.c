#include <game/client.h>
#include <game/request/action/response_fail.h>
#include "player.h"

void player_select_npc(struct GameClient *client, struct Npc *target)
{
        l2_packet *response = game_request_action_fail_response(client);
        l2_packet *encrypted_response = game_service_crypt_packet_encrypt(response, encrypt_key)
        game_client_send_packet(client, 
}
