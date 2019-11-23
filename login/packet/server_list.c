#ifndef L2AUTH_LOGIN_PACKET_SERVER_LIST_C
#define L2AUTH_LOGIN_PACKET_SERVER_LIST_C

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <core/l2_packet.c>
#include <core/byte_buffer.c>
#include <login/server.c>
#include <login/session_key.c>

struct Server* create_dummy_server()
{
        struct Server* server = malloc(sizeof(struct Server));

        server->id = 1;
        server->ip = inet_addr("127.0.0.1");
        server->port = 7777;
        server->age_limit = 0x0f;
        server->pvp = 0x00;
        server->players_count = 600;
        server->max_players = 650;
        server->test = 0x00;

        return server;
}

l2_packet* login_packet_server_list()
{
        l2_packet_type type = 0x04;
        unsigned char server_count = 1;
        unsigned char reserved_space = 0x00;
        struct Server* server = create_dummy_server();
        struct ByteBuffer* buffer = byte_buffer_create();
        l2_packet* packet;

        byte_buffer_append(buffer, &server_count, sizeof(server_count));
        byte_buffer_append(buffer, &reserved_space, sizeof(reserved_space));
        byte_buffer_append(buffer, &server->id, sizeof(server->id));
        byte_buffer_append(buffer, &server->ip, sizeof(server->ip));
        byte_buffer_append(buffer, &server->port, sizeof(server->port));
        byte_buffer_append(buffer, &server->age_limit, sizeof(server->age_limit));
        byte_buffer_append(buffer, &server->pvp, sizeof(server->pvp));
        byte_buffer_append(buffer, &server->players_count, sizeof(server->players_count));
        byte_buffer_append(buffer, &server->max_players, sizeof(server->max_players));
        byte_buffer_append(buffer, &server->test, sizeof(server->test));

        packet = l2_packet_new(
                type,
                byte_buffer_content(buffer),
                (unsigned short) byte_buffer_size(buffer)
        );

        free(server);
        byte_buffer_free(buffer);

        return packet;
}

#endif
