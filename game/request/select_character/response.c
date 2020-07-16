#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_string.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <core/session_key.h>
#include <game/client.h>
#include <game/entity/player.h>
#include "response.h"

l2_packet* game_request_select_character_response
(
        struct GameClient* client,
        struct Player* player
)
{
        assert(client);
        assert(player);

        l2_packet_type type = 0x15;
        byte_builder* buffer = game_client_byte_builder(client, 1000);
        assert(buffer);

        struct L2SessionKey* session = game_client_session(client);
        int playOK1 = session->playOK1;
        
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
                buffer,
                (unsigned char *) name,
                l2_string_calculate_space_from_char(strlen(player->character.name) + 1)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.id,
                sizeof(player->character.id)
        );
        byte_builder_append(buffer, title, sizeof(title));
        byte_builder_append(buffer, (unsigned char *) &playOK1, sizeof(playOK1));
        byte_builder_append(buffer, (unsigned char *) &clan_id, sizeof(clan_id));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.sex,
                sizeof(player->character.sex)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->race_id,
                sizeof(player->race_id)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->class_id,
                sizeof(player->class_id)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->active,
                sizeof(player->active)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.x,
                sizeof(player->character.x)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.y,
                sizeof(player->character.y)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.z,
                sizeof(player->character.z)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.current_hp,
                sizeof(player->character.current_hp)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.current_mp,
                sizeof(player->character.current_mp)
        );
        
        byte_builder_append(buffer, (unsigned char *) &sp, sizeof(sp));
        byte_builder_append(buffer, (unsigned char *) &exp, sizeof(exp));

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.level,
                sizeof(player->character.level)
        );

        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character._int,
                sizeof(player->character._int)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.str,
                sizeof(player->character.str)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.con,
                sizeof(player->character.con)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.men,
                sizeof(player->character.men)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.dex,
                sizeof(player->character.dex)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.wit,
                sizeof(player->character.wit)
        );

        for (int i = 0; i < 30; i++) {
                byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        }

        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &game_time, sizeof(game_time));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));

        return game_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
