#ifndef L2AUTH_LOGIN_GAME_SERVER
#define L2AUTH_LOGIN_GAME_SERVER

#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_socket.c>
#include <core/l2_client.c>
#include <core/l2_packet.c>
#include <game/packet/crypt_init.c>
#include <game/packet/char_list.c>

void log_packet(l2_raw_packet* packet)
{
        l2_raw_packet_size packet_size = l2_raw_packet_get_size(packet);
        char* printable = calloc((size_t) (packet_size * 3), sizeof(char));

        log_info("Received packet");
        log_info("Packet size: %d", packet_size);

        for (int i = 0; i < packet_size; i++)
                sprintf(printable + i * 3, "%02X ", packet[i]);

        log_info("Packet bytes: %s", printable);
        free(printable);
}

void game_server_accept_and_handle_connection
(
        struct L2Socket *server
)
{
        struct L2Client* client = l2_client_new();

        l2_packet *server_packet;
        l2_packet *client_packet;

        unsigned char *decrypted_packet = calloc(65535, sizeof(char));
        int protocol = 0;
        int char_list_sent = 0;

        l2_client_accept(client, server);
        log_info("Gameserver connection accepted");

        while (1) {
                server_packet = NULL;
                client_packet = l2_client_wait_packet_for_gameserver(client);

                if (l2_client_connection_ended(client)) {
                        log_info("Gameserver client connection closed");
                        break;
                }

                if (client_packet) log_packet(client_packet);

                l2_packet_content(
                        client_packet,
                        decrypted_packet,
                        0,
                        l2_packet_get_content_size(client_packet)
                );

                log_info("[ RAW ] Game server first byte %02X", l2_packet_get_type(client_packet));

                switch (l2_packet_get_type(client_packet))
                {
                case 0x00: // protocol version
                        memcpy(&protocol, decrypted_packet, sizeof(int));
                        log_info("Protocol version: %d", protocol);
                        server_packet = game_packet_crypt_init();
                        log_info("Sending crypt init");
                        break;
                default: // auth login
                        log_info("Should be auth login, content size: %d", l2_packet_get_content_size(client_packet));
                        log_info("Should be auth login, type: %c", l2_packet_get_type(client_packet));
                        if (!char_list_sent) {
                                char_list_sent = 1;
                                server_packet = game_packet_char_list();
                                log_info("Sending charlist");
                        }
                        break;
                }

                l2_client_encrypt_and_send_packet(
                        client,
                        server_packet
                );
        }
}

#endif
