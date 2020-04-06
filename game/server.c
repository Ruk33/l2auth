#ifndef L2AUTH_LOGIN_GAME_SERVER_C
#define L2AUTH_LOGIN_GAME_SERVER_C

#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_socket.h>
#include <core/l2_client.h>
#include <core/l2_packet.h>
#include <core/hex.h>
#include <login/dto/session_key.h>
#include <game/crypt.h>
#include <game/packet/crypt_init.h>
#include <game/packet/char_list.h>
#include <game/packet/player_auth_request.h>
#include <game/handler/protocol_version.h>
#include <game/handler/encrypt.h>
#include <game/handler/auth_login.h>
#include <game/handler/new_character.h>
#include <game/handler/create_character.h>
#include <game/handler/select_character.h>
#include <game/handler/d0.h>
#include <game/handler/request_quests.h>
#include <game/handler/enter_world.h>
#include <game/handler/restart.h>
#include <game/server.h>

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
