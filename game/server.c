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
#include <game/handler/encrypt.c>
#include <game/handler/auth_login.c>
#include <game/handler/new_character.c>
#include <game/handler/create_character.c>
#include <game/handler/select_character.c>
#include <game/handler/d0.c>
#include <game/handler/request_quests.c>
#include <game/handler/enter_world.c>
#include <game/handler/restart.c>

void game_server_accept_and_handle_connection
(
        struct L2Socket *server
)
{
        struct L2Client* client = l2_client_new();

        l2_raw_packet *client_raw_packet;

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
        unsigned short sz = 0;

        struct LoginDtoSessionKey* session_key = login_session_key_create();

        l2_client_accept(client, server);
        log_info("Gameserver connection accepted");

        while (1) {
                client_raw_packet = l2_client_wait_packet_for_gameserver(client);

                if (l2_client_connection_ended(client)) {
                        log_info("Gameserver client connection closed");
                        break;
                }

                memcpy(&packet_size, client_raw_packet, sizeof(packet_size));
                sz = packet_size;

                if (packet_size > 1) sz -= 2;

                log_info("Packet size: %d", packet_size);
                log_info("Packet type from client before decrypt %02X", client_raw_packet[2] & 0xff);

                if (enable_decrypt) {
                        game_crypt_decrypt(
                                client_raw_packet + 2,
                                sz,
                                decrypt_key
                        );
                }
                log_info("Packet type from client %02X", client_raw_packet[2] & 0xff);

                switch (client_raw_packet[2] & 0xff)
                {
                case 0x00: // protocol version
                        enable_decrypt = 1;
                        l2_client_send_packet(
                                client,
                                game_handler_protocol_version(client_raw_packet)
                        );
                        break;
                case 0x08: // auth request
                        l2_client_send_packet(
                                client,
                                game_handler_encrypt(
                                        game_handler_auth_login(
                                                client_raw_packet,
                                                session_key
                                        ),
                                        encrypt_key
                                )
                        );
                        break;
                case 0x0e: // new char
                        l2_client_send_packet(
                                client,
                                game_handler_encrypt(
                                        game_handler_new_character(client_raw_packet),
                                        encrypt_key
                                )
                        );
                        break;
                case 0x0b: // create char
                        l2_client_send_packet(
                                client,
                                game_handler_encrypt(
                                        game_handler_create_character(client_raw_packet),
                                        encrypt_key
                                )
                        );
                        l2_client_send_packet(
                                client,
                                game_handler_encrypt(
                                        game_handler_auth_login(client_raw_packet, session_key),
                                        encrypt_key
                                )
                        );
                        break;
                case 0x0d: // selected char (entering world)
                        l2_client_send_packet(
                                client,
                                game_handler_encrypt(
                                        game_handler_select_character(client_raw_packet, session_key->playOK1),
                                        encrypt_key
                                )
                        );
                        break;
                case 0xd0: // request auto ss or bsps
                        game_handler_d0(client_raw_packet);
                        break;
                case 0x63: // request quest list
                        l2_client_send_packet(
                                client,
                                game_handler_encrypt(
                                        game_handler_request_quests(client_raw_packet),
                                        encrypt_key
                                )
                        );
                        break;
                case 0x03: // enter world
                        l2_client_send_packet(
                                client,
                                game_handler_encrypt(
                                        game_handler_enter_world(client_raw_packet),
                                        encrypt_key
                                )
                        );
                        break;
                case 0x46: // restart
                        l2_client_send_packet(
                                client,
                                game_handler_encrypt(
                                        game_handler_restart(client_raw_packet),
                                        encrypt_key
                                )
                        );
                        l2_client_send_packet(
                                client,
                                game_handler_encrypt(
                                        game_handler_auth_login(client_raw_packet, session_key),
                                        encrypt_key
                                )
                        );
                        break;
                default:
                        log_info("default case to be implemented");
                        break;
                }
        }
}

#endif
