#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/connection_thread.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_raw_packet.h>
#include <game/crypt.h>
#include <game/packet/client_type.h>
#include <game/handler/protocol_version.h>
#include <game/handler/auth_login.h>
#include <game/handler/new_character.h>
#include <game/handler/create_character.h>
#include <game/handler/select_character.h>
#include <game/handler/d0.h>
#include <game/handler/request_quests.h>
#include <game/handler/enter_world.h>
#include <game/handler/restart.h>
#include <game/handler/move_backwards_to_location.h>
// #include <game/handler/validate_position.h>
#include <game/handler/client.h>

int game_handler_client(struct ConnectionThread* conn)
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
                game_crypt_decrypt(
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
        case GAME_PACKET_CLIENT_TYPE_PROTOCOL_VERSION:
                conn->is_encrypted = 1;
                game_handler_protocol_version(
                        server,
                        client,
                        client_packet
                );
                break;
        case GAME_PACKET_CLIENT_TYPE_MOVE_BACKWARDS_TO_LOCATION:
                game_handler_move_backwards_to_location(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        // case GAME_PACKET_CLIENT_TYPE_VALIDATE_POS:
        //         game_handler_validate_position(
        //                 server,
        //                 client,
        //                 client_packet,
        //                 conn->encrypt_key
        //         );
        //         break;
        case GAME_PACKET_CLIENT_TYPE_AUTH_REQUEST:
                game_handler_auth_login(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_PACKET_CLIENT_TYPE_NEW_CHAR:
                game_handler_new_character(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_PACKET_CLIENT_TYPE_CREATE_CHAR:
                game_handler_create_character(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_PACKET_CLIENT_TYPE_SELECTED_CHAR:
                game_handler_select_character(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_PACKET_CLIENT_TYPE_REQUEST_AUTO_SS_BSPS:
                game_handler_d0(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_PACKET_CLIENT_TYPE_REQUEST_QUEST_LIST:
                game_handler_request_quests(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_PACKET_CLIENT_TYPE_ENTER_WORLD:
                game_handler_enter_world(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        case GAME_PACKET_CLIENT_TYPE_RESTART:
                game_handler_restart(
                        server,
                        client,
                        client_packet,
                        conn->encrypt_key
                );
                break;
        default:
                log_error(
                        "Oops, packet %02X not impleted yet",
                        packet_type
                );
                break;
        }

        return 0;
}
