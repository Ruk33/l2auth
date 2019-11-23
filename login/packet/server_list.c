#ifndef L2AUTH_LOGIN_PACKET_SERVER_LIST_C
#define L2AUTH_LOGIN_PACKET_SERVER_LIST_C

#include <string.h>
#include <arpa/inet.h>
#include <core/l2_packet.c>
#include <core/byte_buffer.c>
#include <login/session_key.c>

l2_packet* login_packet_server_list()
{
        l2_packet_type type = 0x04;
        unsigned char server_count = 1;
        unsigned char reserved_space = 0x00;
        unsigned char server_id = 1;
        in_addr_t server_ip = inet_addr("127.0.0.1");
        int server_port = 7777;
        unsigned char server_age_limit = 0x0f;
        unsigned char server_pvp = 0x01;
        unsigned short server_players = 600;
        unsigned short server_max_players = 650;
        unsigned char server_test = 0x00;
        struct ByteBuffer* buffer = byte_buffer_create();
        l2_packet* packet;

        byte_buffer_append(buffer, &server_count, sizeof(server_count));
        byte_buffer_append(buffer, &reserved_space, sizeof(reserved_space));
        byte_buffer_append(buffer, &server_id, sizeof(server_id));
        byte_buffer_append(buffer, &server_ip, sizeof(server_ip));
        byte_buffer_append(buffer, &server_port, sizeof(server_port));
        byte_buffer_append(buffer, &server_age_limit, sizeof(server_age_limit));
        byte_buffer_append(buffer, &server_pvp, sizeof(server_pvp));
        byte_buffer_append(buffer, &server_players, sizeof(server_players));
        byte_buffer_append(buffer, &server_max_players, sizeof(server_max_players));
        byte_buffer_append(buffer, &server_test, sizeof(server_test));

        packet = l2_packet_new(
                type,
                byte_buffer_content(buffer),
                (unsigned short) byte_buffer_size(buffer)
        );

        byte_buffer_free(buffer);

        return packet;
}

#endif
