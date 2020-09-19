#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <core/l2_string.h>
#include "../../storage/conn.h"
#include "../../storage/character.h"
#include "../../client.h"
#include "../../entity/player.h"
#include "../../packet_builder.h"
#include "response.h"

static void char_to_buffer
(byte_builder *buffer, int play_ok_1, struct Player *player)
{
        assert(buffer);
        assert(player);

        int clan_id = 0x00;

        int empty_space = 0x00;

        int sp = 0;
        int exp = 0;
        int karma = 0;

        int under_obj_id = 0;
        int r_ear_obj_id = 0;
        int l_ear_obj_id = 0;
        int neck_obj_id = 0;
        int r_finger_obj_id = 0;
        int l_finger_obj_id = 0;
        int head_obj_id = 0;
        int r_hand_obj_id = 0;
        int l_hand_obj_id = 0;
        int gloves_obj_id = 0;
        int chest_obj_id = 0;
        int legs_obj_id = 0;
        int feet_obj_id = 0;
        int back_obj_id = 0;
        int lr_hand_obj_id = 0;
        int hair_obj_id = 0;

        int under = 0;
        int r_ear = 0;
        int l_ear = 0;
        int neck = 0;
        int r_finger = 0;
        int l_finger = 0;
        int head = 0;
        int r_hand = 0;
        int l_hand = 0;
        int gloves = 0;
        int chest = 0;
        int legs = 0;
        int feet = 0;
        int back = 0;
        int lr_hand = 0;
        int hair = 0;

        int delete_days = 0;

        int auto_select = 0;
        char enchant_effect = 0;

        l2_string name[28];
        size_t name_size = 0;
        size_t name_len = 0;

        memset(name, 0, 28);

        name_len = strlen(player->character.name) + 1;
        l2_string_from_char(name, player->character.name, name_len);
        name_size = l2_string_calculate_space_from_char(name_len);
        
        byte_builder_append(buffer, name, name_size);

        byte_builder_append_int(buffer, &player->character.id);

        byte_builder_append(buffer, name, name_size);

        byte_builder_append_int(buffer, &play_ok_1);

        byte_builder_append_int(buffer, &clan_id);

        byte_builder_append_int(buffer, &empty_space);

        byte_builder_append_int(buffer, &player->character.sex);
        byte_builder_append_int(buffer, &player->race_id);
        byte_builder_append_int(buffer, &player->class_id);
        byte_builder_append_int(buffer, &player->active);

        byte_builder_append_int(
                buffer,
                &empty_space
                // &character->current_location.x
        );

        byte_builder_append_int(
                buffer,
                &empty_space
                // &character->current_location.y
        );

        byte_builder_append_int(
                buffer,
                &empty_space
                // &character->current_location.z
        );

        byte_builder_append_double(buffer, &player->character.current_hp);
        
        byte_builder_append_double(buffer, &player->character.current_mp);

        byte_builder_append_int(buffer, &sp);
        byte_builder_append_int(buffer, &exp);

        byte_builder_append_int(buffer, &player->character.level);

        byte_builder_append_int(buffer, &karma);

        byte_builder_append_int(buffer, &empty_space);
        byte_builder_append_int(buffer, &empty_space);
        byte_builder_append_int(buffer, &empty_space);
        byte_builder_append_int(buffer, &empty_space);
        byte_builder_append_int(buffer, &empty_space);
        byte_builder_append_int(buffer, &empty_space);
        byte_builder_append_int(buffer, &empty_space);
        byte_builder_append_int(buffer, &empty_space);
        byte_builder_append_int(buffer, &empty_space);

        byte_builder_append_int(buffer, &under_obj_id);
        byte_builder_append_int(buffer, &r_ear_obj_id);
        byte_builder_append_int(buffer, &l_ear_obj_id);
        byte_builder_append_int(buffer, &neck_obj_id);
        byte_builder_append_int(buffer, &r_finger_obj_id);
        byte_builder_append_int(buffer, &l_finger_obj_id);
        byte_builder_append_int(buffer, &head_obj_id);
        byte_builder_append_int(buffer, &r_hand_obj_id);
        byte_builder_append_int(buffer, &l_hand_obj_id);
        byte_builder_append_int(buffer, &gloves_obj_id);
        byte_builder_append_int(buffer, &chest_obj_id);
        byte_builder_append_int(buffer, &legs_obj_id);
        byte_builder_append_int(buffer, &feet_obj_id);
        byte_builder_append_int(buffer, &back_obj_id);
        byte_builder_append_int(buffer, &lr_hand_obj_id);
        byte_builder_append_int(buffer, &hair_obj_id);

        byte_builder_append_int(buffer, &under);
        byte_builder_append_int(buffer, &r_ear);
        byte_builder_append_int(buffer, &l_ear);
        byte_builder_append_int(buffer, &neck);
        byte_builder_append_int(buffer, &r_finger);
        byte_builder_append_int(buffer, &l_finger);
        byte_builder_append_int(buffer, &head);
        byte_builder_append_int(buffer, &r_hand);
        byte_builder_append_int(buffer, &l_hand);
        byte_builder_append_int(buffer, &gloves);
        byte_builder_append_int(buffer, &chest);
        byte_builder_append_int(buffer, &legs);
        byte_builder_append_int(buffer, &feet);
        byte_builder_append_int(buffer, &back);
        byte_builder_append_int(buffer, &lr_hand);
        byte_builder_append_int(buffer, &hair);

        byte_builder_append_int(buffer, &player->hair_style_id);
        byte_builder_append_int(buffer, &player->hair_color_id);
        byte_builder_append_int(buffer, &player->face);

        byte_builder_append_double(buffer, &player->character.hp);
        byte_builder_append_double(buffer, &player->character.mp);

        byte_builder_append_int(buffer, &delete_days);

        byte_builder_append_int(buffer, &player->class_id);

        byte_builder_append_int(buffer, &auto_select);
        byte_builder_append_char(buffer, &enchant_effect);
}

l2_packet *auth_login_response
(struct Client *client)
{
        assert(client);

        l2_packet_type type = 0x13;
        l2_packet *response = NULL;

        struct L2SessionKey *key = NULL;

        struct Player **characters = NULL;
        int characters_count = 0;

        size_t buf_size = 0;
        byte_builder *buf = NULL;
        byte_builder *builder = NULL;

        key = client_session(client);

        buf_size = byte_builder_calculate_size(1024);
        buf = client_alloc_mem(client, buf_size);
        builder = byte_builder_init(buf, buf_size);

        characters = storage_characters_all(client);

        // I know, totally barbaric and I should be
        // crucified because of it, but it's temporary
        while (characters[characters_count]) {
                characters_count++;
        }

        byte_builder_append_int(builder, &characters_count);

        for (int i = 0; characters[i]; i++) {
                char_to_buffer(builder, key->playOK1, characters[i]);
                client_free_mem(client, characters[i]);
        }

        response = packet_builder_new(
                client,
                type,
                builder,
                byte_builder_length(builder)
        );

        client_free_mem(client, buf);
        client_free_mem(client, characters);
        client_free_mem(client, key);

        return response;
}
