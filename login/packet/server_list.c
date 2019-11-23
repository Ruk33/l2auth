#ifndef L2AUTH_LOGIN_PACKET_SERVER_LIST_C
#define L2AUTH_LOGIN_PACKET_SERVER_LIST_C

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <core/l2_packet.c>
#include <login/session_key.c>

l2_packet* login_packet_server_list()
{
        l2_packet_type type = 0x04;
        unsigned char server_count = 1;
        unsigned char reserved_space = 0x00;
        unsigned char server_id = 2;
        in_addr_t server_ip = inet_addr("127.0.0.1");
        int server_port = 7777;
        unsigned char server_age_limit = 0x0f;
        unsigned char server_pvp = 0x01;
        unsigned short server_players = 600;
        unsigned short server_max_players = 650;
        unsigned char server_test = 0x00;
        size_t content_size = (
                sizeof(server_count) +
                sizeof(reserved_space) +
                sizeof(server_id) +
                sizeof(server_ip) +
                sizeof(server_port) +
                sizeof(server_age_limit) +
                sizeof(server_pvp) +
                sizeof(server_players) +
                sizeof(server_max_players) +
                sizeof(server_test)
        );
        unsigned char* content = calloc(content_size, sizeof(char));
        size_t cursor = 0;
        l2_packet* packet;

        content[cursor] = server_count;
        cursor += sizeof(server_count);

        content[cursor] = reserved_space;
        cursor += sizeof(reserved_space);

        content[cursor] = server_id;
        cursor += sizeof(server_id);

        memcpy(content + cursor, &server_ip, sizeof(server_ip));
        cursor += sizeof(server_ip);

        memcpy(content + cursor, &server_port, sizeof(server_port));
        cursor += sizeof(server_port);

        content[cursor] = server_age_limit;
        cursor += sizeof(server_age_limit);

        content[cursor] = server_pvp;
        cursor += sizeof(server_pvp);

        memcpy(content + cursor, &server_players, sizeof(server_players));
        cursor += sizeof(server_players);

        memcpy(content + cursor, &server_max_players, sizeof(server_max_players));
        cursor += sizeof(server_max_players);

        content[cursor] = server_test;
        cursor += sizeof(server_test);

        packet = l2_packet_new(
                type,
                content,
                (unsigned short) content_size
        );

        free(content);

        return packet;
}

#endif
