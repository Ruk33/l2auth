#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <core/byte_builder.h>
#include <login/dto/server.h>
#include <core/session_key.h>
#include <login/packet/server_list.h>

void initialize_dummy_server
(
        struct LoginDtoServer* server,
        unsigned char server_id,
        unsigned short players_count
)
{
        assert(server);

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
}

l2_packet* login_packet_server_list(struct L2Client* client)
{
        assert(client);
        l2_packet_type type = 0x04;
        unsigned char server_count = 1;
        unsigned char reserved_space = 0x00;
        struct LoginDtoServer* bartz = l2_client_alloc_temp_mem(
                client,
                sizeof(struct LoginDtoServer)
        );
        /*
         * (franco.montenegro)
         * Check why inetaddr does not work here
         */
        unsigned char ip[] = { 0, 0, 0, 0 };
        byte_builder* buffer = l2_client_byte_builder(
                client,
                sizeof(server_count) +
                sizeof(reserved_space) +
                sizeof(ip) +
                sizeof(struct LoginDtoServer)
        );

        initialize_dummy_server(bartz, 1, 400);

        byte_builder_append(
                buffer,
                (unsigned char *) &server_count,
                sizeof(server_count)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &reserved_space,
                sizeof(reserved_space)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &bartz->id,
                sizeof(bartz->id)
        );
        byte_builder_append(buffer, ip, sizeof(ip));
        byte_builder_append(
                buffer,
                (unsigned char *) &bartz->port,
                sizeof(bartz->port)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &bartz->age_limit,
                sizeof(bartz->age_limit)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &bartz->pvp,
                sizeof(bartz->pvp)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &bartz->players_count,
                sizeof(bartz->players_count)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &bartz->max_players,
                sizeof(bartz->max_players)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &bartz->status,
                sizeof(bartz->status)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &bartz->extra,
                sizeof(bartz->extra)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &bartz->brackets,
                sizeof(bartz->brackets)
        );

        return l2_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
