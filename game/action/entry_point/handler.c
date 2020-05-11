#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/connection_thread.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_raw_packet.h>
#include <game/service/crypt/decrypt.h>
#include <game/action/request_type.h>
#include <game/action/protocol_version/handler.h>
#include <game/action/auth_login/handler.h>
#include <game/action/new_character/handler.h>
#include <game/action/create_character/handler.h>
#include <game/action/select_character/handler.h>
#include <game/action/d0/handler.h>
#include <game/action/request_quests/handler.h>
#include <game/action/enter_world/handler.h>
#include <game/action/restart/handler.h>
#include <game/action/move/handler.h>
#include <game/action/validate_position/handler.h>
#include <game/action/say/handler.h>
#include "handler.h"

int game_action_entry_point_handler(struct ConnectionThread* conn)
{
        assert(conn);

        struct L2Server* server = conn->server;
        struct L2Client* client = conn->client;

        l2_raw_packet *client_packet = l2_client_wait_packet(client);

        unsigned short packet_size = 0;
        l2_packet_type packet_type;

        if (l2_client_connection_ended(client)) return 1;

        memcpy(&packet_size, client_packet, sizeof(packet_size));
        if (packet_size > 1) {
                packet_size = (unsigned short) (packet_size - 2);
        }

        if (conn->is_encrypted) {
                game_service_crypt_decrypt(
                        client_packet + 2,
                        packet_size,
                        conn->decrypt_key
                );
        }

        /*
         * (franco.montenegro)
         * Check why l2_packet_get_type is not working here
         */
        packet_type = (l2_packet_type) (client_packet[2] & 0xff);

        switch (packet_type) {
        case GAME_ACTION_REQUEST_TYPE_PROTOCOL_VERSION:
                conn->is_encrypted = 1;
                game_action_protocol_version_handler(
                        server,
                        client,
                        client_packet
                );
                break;
        case GAME_ACTION_REQUEST_TYPE_MOVE_BACKWARDS_TO_LOCATION:
                game_action_move_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_ACTION_REQUEST_TYPE_VALIDATE_POS:
                game_action_validate_position_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_ACTION_REQUEST_TYPE_AUTH_REQUEST:
                game_action_auth_login_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_ACTION_REQUEST_TYPE_NEW_CHAR:
                game_action_new_character_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_ACTION_REQUEST_TYPE_CREATE_CHAR:
                game_action_create_character_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_ACTION_REQUEST_TYPE_SELECTED_CHAR:
                game_action_select_character_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_ACTION_REQUEST_TYPE_REQUEST_AUTO_SS_BSPS:
                game_action_d0_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_ACTION_REQUEST_TYPE_REQUEST_QUEST_LIST:
                game_action_request_quests_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_ACTION_REQUEST_TYPE_ENTER_WORLD:
                game_action_enter_world_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_ACTION_REQUEST_TYPE_RESTART:
                game_action_restart_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_ACTION_REQUEST_TYPE_SAY:
                game_action_say_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        default:
                log_error(
                        "Oops, packet %02X not implemented yet",
                        packet_type
                );
                break;
        }

        return 0;
}
