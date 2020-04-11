#ifndef L2AUTH_GAME_HANDLER_CLIENT_C
#define L2AUTH_GAME_HANDLER_CLIENT_C

#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_server.h>
#include <core/l2_client.h>
#include <core/l2_raw_packet.h>
#include <game/crypt.h>
#include <game/packet/client_type.h>
#include <game/handler/protocol_version.h>
#include <game/handler/auth_login.h>
#include <game/handler/new_character.h>
#include <game/handler/create_character.h>
#include <game/handler/client.h>

void game_handler_client
(
        struct L2Server* server,
        struct L2Client* client
)
{
        assert(server);
        assert(client);

        l2_raw_packet *client_packet = NULL;

        unsigned char encrypt_key[] = {
                0x94,
                0x35,
                0x00,
                0x00,
                0xa1,
                0x6c,
                0x54,
                0x87,
        };

        unsigned char decrypt_key[] = {
                0x94,
                0x35,
                0x00,
                0x00,
                0xa1,
                0x6c,
                0x54,
                0x87,
        };

        int enable_decrypt = 0;
        unsigned short packet_size = 0;

        log_info("Handling new client from gameserver");
        
        while (1) {
                client_packet = l2_client_wait_packet(client);

                if (l2_client_connection_ended(client)) break;

                memcpy(&packet_size, client_packet, sizeof(packet_size));
                if (packet_size > 1) {
                        packet_size = (unsigned short) (packet_size - 2);
                }

                if (enable_decrypt) {
                        game_crypt_decrypt(
                                client_packet + 2,
                                packet_size,
                                decrypt_key
                        );
                }

                /*
                 * (franco.montenegro)
                 * Check why l2_packet_get_type is not working here
                 */
                switch (client_packet[2] & 0xff) {
                case GAME_PACKET_CLIENT_TYPE_PROTOCOL_VERSION:
                        enable_decrypt = 1;
                        game_handler_protocol_version(
                                server,
                                client,
                                client_packet
                        );
                        break;
                case GAME_PACKET_CLIENT_TYPE_AUTH_REQUEST:
                        game_handler_auth_login(
                                server,
                                client,
                                client_packet,
                                encrypt_key
                        );
                        break;
                case GAME_PACKET_CLIENT_TYPE_NEW_CHAR:
                        game_handler_new_character(
                                server,
                                client,
                                client_packet,
                                encrypt_key
                        );
                        break;
                case GAME_PACKET_CLIENT_TYPE_CREATE_CHAR:
                        game_handler_create_character(
                                server,
                                client,
                                client_packet,
                                encrypt_key
                        );
                        break;
                case GAME_PACKET_CLIENT_TYPE_SELECTED_CHAR:
                        break;
                case GAME_PACKET_CLIENT_TYPE_REQUEST_AUTO_SS_BSPS:
                        break;
                case GAME_PACKET_CLIENT_TYPE_REQUEST_QUEST_LIST:
                        break;
                case GAME_PACKET_CLIENT_TYPE_ENTER_WORLD:
                        break;
                case GAME_PACKET_CLIENT_TYPE_RESTART:
                        break;
                default:
                        log_info("Oops, non implemented packet");
                        break;
                }

        }

        log_info("Gameserver client connection closed");
}

#endif
