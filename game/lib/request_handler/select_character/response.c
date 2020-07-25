#include <string.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <core/session_key.h>
#include <core/l2_string.h>
#include "../../entity/pc.h"
#include "../../client.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *select_character_response(struct Client *client)
{
        l2_packet_type type = 0x15;
        l2_packet *response;

        struct L2SessionKey* session = client_session(client);
        struct Pc *player = client_character(client);

        size_t buf_size = byte_builder_calculate_size(1024);
        byte_builder* buf = client_alloc_mem(client, buf_size);
        byte_builder *builder = byte_builder_init(buf, buf_size);

        unsigned char title[] = { 0, 0 };
        unsigned int clan_id = 0;
        unsigned int empty = 0;
        unsigned int sp = 20;
        unsigned int exp = 10;
        unsigned int game_time = 10;

        l2_string name[28];

        memset(name, 0, 28);
        l2_string_from_char(name, player->character.name, strlen(player->character.name));
        
        byte_builder_append(
                builder,
                name,
                l2_string_calculate_space_from_char(strlen(player->character.name) + 1)
        );

        byte_builder_append(builder, &player->character.id, sizeof(player->character.id));
        byte_builder_append(builder, title, sizeof(title));
        byte_builder_append(builder, &session->playOK1, sizeof(session->playOK1));
        byte_builder_append(builder, &clan_id, sizeof(clan_id));

        byte_builder_append(builder, &empty, sizeof(empty));

        byte_builder_append(builder, &player->character.sex, sizeof(player->character.sex));
        byte_builder_append(builder, &player->race_id, sizeof(player->race_id));
        byte_builder_append(builder, &player->class_id, sizeof(player->class_id));
        byte_builder_append(builder, &player->active, sizeof(player->active));

        byte_builder_append(builder, &player->character.x, sizeof(player->character.x));
        byte_builder_append(builder, &player->character.y, sizeof(player->character.y));
        byte_builder_append(builder, &player->character.z, sizeof(player->character.z));

        byte_builder_append(builder, &player->character.current_hp, sizeof(player->character.current_hp));
        byte_builder_append(builder, &player->character.current_mp, sizeof(player->character.current_mp));

        byte_builder_append(builder, &sp, sizeof(sp));
        byte_builder_append(builder, &exp, sizeof(exp));

        byte_builder_append(builder, &player->character.level, sizeof(player->character.level));

        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));

        byte_builder_append(builder, &player->character._int, sizeof(player->character._int));
        byte_builder_append(builder, &player->character.str, sizeof(player->character.str));
        byte_builder_append(builder, &player->character.con, sizeof(player->character.con));
        byte_builder_append(builder, &player->character.men, sizeof(player->character.men));
        byte_builder_append(builder, &player->character.dex, sizeof(player->character.dex));
        byte_builder_append(builder, &player->character.wit, sizeof(player->character.wit));

        for (int i = 0; i < 30; i++) {
                byte_builder_append(builder, &empty, sizeof(empty));
        }

        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &game_time, sizeof(game_time));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));
        byte_builder_append(builder, &empty, sizeof(empty));

        response = packet_builder_new(client, type, builder, byte_builder_length(builder));

        client_free_mem(client, player);
        client_free_mem(client, session);
        client_free_mem(client, buf);

        return response;
}
