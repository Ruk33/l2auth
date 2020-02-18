#ifndef L2AUTH_LOGIN_PACKET_SERVER_LIST_C
#define L2AUTH_LOGIN_PACKET_SERVER_LIST_C

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <core/l2_packet.c>
#include <core/byte_buffer.c>
#include <login/dto/server.c>
#include <login/dto/session_key.c>

struct LoginDtoServer* create_dummy_server
(       
        unsigned char server_id,
        unsigned short players_count
)
{
        struct LoginDtoServer* server = calloc(1, sizeof(struct LoginDtoServer));
        unsigned char online = 0x01;
        //int clock = 0x02;
        unsigned char hide_brackets = 0x00;

        server->id = server_id;
        //server->ip = inet_addr("127.0.0.1");
        server->port = 7777;
        server->age_limit = 0x0f;
        server->pvp = 0x00;
        server->players_count = players_count;
        server->max_players = 650;
        server->status = online;
        //server->extra = 0x00 | clock;
        server->extra = 0x00;
        server->brackets = hide_brackets;

        return server;
}

l2_packet* login_packet_server_list()
{
        l2_packet_type type = 0x04;
        unsigned char server_count = 1;
        unsigned char reserved_space = 0x00;
        struct LoginDtoServer* bartz = create_dummy_server(1, 400);
        struct ByteBuffer* buffer = byte_buffer_create();
        unsigned char ip[] = { 0, 0, 0, 0 }; // TODO check why inetaddr doest not work here
        l2_packet* packet;

        byte_buffer_append(buffer, &server_count, sizeof(server_count));
        byte_buffer_append(buffer, &reserved_space, sizeof(reserved_space));
        byte_buffer_append(buffer, &bartz->id, sizeof(bartz->id));
        byte_buffer_append(buffer, &ip, sizeof(ip));
        byte_buffer_append(buffer, &bartz->port, sizeof(bartz->port));
        byte_buffer_append(buffer, &bartz->age_limit, sizeof(bartz->age_limit));
        byte_buffer_append(buffer, &bartz->pvp, sizeof(bartz->pvp));
        byte_buffer_append(buffer, &bartz->players_count, sizeof(bartz->players_count));
        byte_buffer_append(buffer, &bartz->max_players, sizeof(bartz->max_players));
        byte_buffer_append(buffer, &bartz->status, sizeof(bartz->status));
        byte_buffer_append(buffer, &bartz->extra, sizeof(bartz->extra));
        byte_buffer_append(buffer, &bartz->brackets, sizeof(bartz->brackets));

        packet = l2_packet_new(
                type,
                byte_buffer_content(buffer),
                (unsigned short) byte_buffer_size(buffer)
        );

        free(bartz);
        byte_buffer_free(buffer);

        return packet;
}

#endif
