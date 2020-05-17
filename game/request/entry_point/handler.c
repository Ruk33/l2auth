#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <game/connection.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/crypt/decrypt.h>
#include <game/request/type.h>
#include <game/request/protocol_version/handler.h>
#include <game/request/auth_login/handler.h>
#include <game/request/new_character/handler.h>
#include <game/request/create_character/handler.h>
#include <game/request/select_character/handler.h>
#include <game/request/d0/handler.h>
#include <game/request/quest_list/handler.h>
#include <game/request/enter_world/handler.h>
#include <game/request/restart/handler.h>
#include <game/request/move/handler.h>
#include <game/request/validate_position/handler.h>
#include <game/request/say/handler.h>
#include "handler.h"

int game_request_entry_point_handler(struct GameConnection* conn)
{
        assert(conn);

        struct GameServer* server = conn->server;
        struct GameClient* client = conn->client;

        l2_raw_packet *client_packet = game_client_wait_packet(client);

        unsigned short packet_size = 0;
        l2_packet_type packet_type;

        if (game_client_connection_ended(client)) return 1;

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

        log_error("--- Packet type %02X", packet_type);
        log_error("--- Packet type without crap %02X", client_packet[2]);

        switch (packet_type) {
        case  GAME_REQUEST_TYPE_PROTOCOL_VERSION:
                conn->is_encrypted = 1;
                game_request_protocol_version_handler(
                        server,
                        client,
                        client_packet
                );
                break;
        case  GAME_REQUEST_TYPE_MOVE_BACKWARDS_TO_LOCATION:
                game_request_move_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case  GAME_REQUEST_TYPE_VALIDATE_POS:
                game_request_validate_position_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case  GAME_REQUEST_TYPE_AUTH_REQUEST:
                game_request_auth_login_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case  GAME_REQUEST_TYPE_NEW_CHAR:
                game_request_new_character_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case  GAME_REQUEST_TYPE_CREATE_CHAR:
                game_request_create_character_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case  GAME_REQUEST_TYPE_SELECTED_CHAR:
                game_request_select_character_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case  GAME_REQUEST_TYPE_REQUEST_AUTO_SS_BSPS:
                game_request_d0_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case  GAME_REQUEST_TYPE_REQUEST_QUEST_LIST:
                game_request_quest_list_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case  GAME_REQUEST_TYPE_ENTER_WORLD:
                game_request_enter_world_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case  GAME_REQUEST_TYPE_RESTART:
                game_request_restart_handler(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case  GAME_REQUEST_TYPE_SAY:
                game_request_say_handler(
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
