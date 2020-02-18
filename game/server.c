#ifndef L2AUTH_LOGIN_GAME_SERVER
#define L2AUTH_LOGIN_GAME_SERVER

#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_socket.c>
#include <core/l2_client.c>
#include <core/l2_packet.c>
#include <core/hex.c>
#include <login/dto/session_key.c>
#include <game/crypt.c>
#include <game/packet/crypt_init.c>
#include <game/packet/char_list.c>
#include <game/packet/player_auth_request.c>
#include <game/handler/protocol_version.c>
#include <game/handler/auth_login.c>

void game_server_accept_and_handle_connection
(
        struct L2Socket *server
)
{
        struct L2Client* client = l2_client_new();

        l2_raw_packet *client_raw_packet;

        unsigned char blowfish_key[40];

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

        hex_generate(blowfish_key, sizeof(blowfish_key));
        l2_client_accept(client, server);
        log_info("Gameserver connection accepted");

        while (1) {
                client_raw_packet = l2_client_wait_packet_for_gameserver(client);

                if (l2_client_connection_ended(client)) {
                        log_info("Gameserver client connection closed");
                        break;
                }

                if (client_raw_packet[2] & 0xff)
                        game_crypt_decrypt(client_raw_packet + 2, l2_raw_packet_get_size(client_raw_packet), decrypt_key);

                game_crypt_checksum(client_raw_packet + 2, l2_raw_packet_get_size(client_raw_packet));

                switch (client_raw_packet[2] & 0xff)
                {
                case 0x00: // protocol version
                        l2_client_send_packet(
                                client,
                                game_handler_protocol_version(client_raw_packet)
                        );
                        break;
                case 0x08: // auth request
                        l2_client_send_packet(
                                client,
                                game_handler_auth_login(client_raw_packet)
                        );
                        break;
                default:
                        log_info("default case to be implemented");
                        break;
                }
        }
}

#endif
