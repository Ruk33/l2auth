#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_string.h>
#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <core/byte_builder.h>
#include <core/session_key.h>
#include <game/entity/character.h>
#include "response.h"

l2_packet* game_action_select_character_response
(
        struct L2Client* client,
        struct GameEntityCharacter* character
)
{
        assert(client);

        l2_packet_type type = 0x15;
        byte_builder* buffer = l2_client_byte_builder(client, 1000);

        struct L2DtoSessionKey* session = l2_client_session(client);
        int playOK1 = session->playOK1;
        
        unsigned char title[] = { 0, 0 };
        unsigned int clan_id = 0;
        unsigned int empty = 0;
        unsigned int sp = 20;
        unsigned int exp = 10;
        unsigned int game_time = 10;

        l2_string name[28];

        memset(name, 0, 28);
        l2_string_from_char(name, character->name, strlen(character->name));
        
        byte_builder_append(
                buffer,
                (unsigned char *) name,
                l2_string_calculate_space_from_char(strlen(character->name) + 1)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->char_id,
                sizeof(character->char_id)
        );
        byte_builder_append(buffer, title, sizeof(title));
        byte_builder_append(buffer, (unsigned char *) &playOK1, sizeof(playOK1));
        byte_builder_append(buffer, (unsigned char *) &clan_id, sizeof(clan_id));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));

        byte_builder_append(
                buffer,
                (unsigned char *) &character->sex,
                sizeof(character->sex)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->race_id,
                sizeof(character->race_id)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->class_id,
                sizeof(character->class_id)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->active,
                sizeof(character->active)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->current_location.x,
                sizeof(character->current_location.x)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->current_location.y,
                sizeof(character->current_location.y)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->current_location.z,
                sizeof(character->current_location.z)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->current_hp,
                sizeof(character->current_hp)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->current_mp,
                sizeof(character->current_mp)
        );
        
        byte_builder_append(buffer, (unsigned char *) &sp, sizeof(sp));
        byte_builder_append(buffer, (unsigned char *) &exp, sizeof(exp));

        byte_builder_append(
                buffer,
                (unsigned char *) &character->level,
                sizeof(character->level)
        );

        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));

        byte_builder_append(
                buffer,
                (unsigned char *) &character->_int,
                sizeof(character->_int)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->str,
                sizeof(character->str)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->con,
                sizeof(character->con)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->men,
                sizeof(character->men)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->dex,
                sizeof(character->dex)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->wit,
                sizeof(character->wit)
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

        return l2_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
