#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_string.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <core/session_key.h>
#include <game/client.h>
#include <game/service/character/list.h>
#include <game/entity/player.h>
#include "response.h"

void game_packet_char_list_add_char_to_buffer
(
        byte_builder* buffer,
        int play_ok_1,
        struct Player* player
)
{
        char login[] = { 'r', 0, 'u', 0, 'k', 0, 'e', 0, 0, 0 };

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
        unsigned char enchant_effect = 0;

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

        byte_builder_append(buffer, (unsigned char *) login, sizeof(login));

        byte_builder_append(
                buffer,
                (unsigned char *) &play_ok_1,
                sizeof(play_ok_1)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &clan_id,
                sizeof(clan_id)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &empty_space,
                sizeof(empty_space)
        );

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
                (unsigned char * ) &empty_space,
                sizeof(empty_space)
                // (unsigned char *) &character->current_location.x,
                // sizeof(character->current_location.x)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &empty_space,
                sizeof(empty_space)
                // (unsigned char *) &character->current_location.y,
                // sizeof(character->current_location.y)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &empty_space,
                sizeof(empty_space)
                // (unsigned char *) &character->current_location.z,
                // sizeof(character->current_location.z)
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

        byte_builder_append(buffer, (unsigned char *) &karma, sizeof(karma));

        byte_builder_append(buffer, (unsigned char *) &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, (unsigned char *) &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, (unsigned char *) &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, (unsigned char *) &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, (unsigned char *) &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, (unsigned char *) &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, (unsigned char *) &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, (unsigned char *) &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, (unsigned char *) &empty_space, sizeof(empty_space));

        byte_builder_append(buffer, (unsigned char *) &under_obj_id, sizeof(under_obj_id));
        byte_builder_append(buffer, (unsigned char *) &r_ear_obj_id, sizeof(r_ear_obj_id));
        byte_builder_append(buffer, (unsigned char *) &l_ear_obj_id, sizeof(l_ear_obj_id));
        byte_builder_append(buffer, (unsigned char *) &neck_obj_id, sizeof(neck_obj_id));
        byte_builder_append(buffer, (unsigned char *) &r_finger_obj_id, sizeof(r_finger_obj_id));
        byte_builder_append(buffer, (unsigned char *) &l_finger_obj_id, sizeof(l_finger_obj_id));
        byte_builder_append(buffer, (unsigned char *) &head_obj_id, sizeof(head_obj_id));
        byte_builder_append(buffer, (unsigned char *) &r_hand_obj_id, sizeof(r_hand_obj_id));
        byte_builder_append(buffer, (unsigned char *) &l_hand_obj_id, sizeof(l_hand_obj_id));
        byte_builder_append(buffer, (unsigned char *) &gloves_obj_id, sizeof(gloves_obj_id));
        byte_builder_append(buffer, (unsigned char *) &chest_obj_id, sizeof(chest_obj_id));
        byte_builder_append(buffer, (unsigned char *) &legs_obj_id, sizeof(legs_obj_id));
        byte_builder_append(buffer, (unsigned char *) &feet_obj_id, sizeof(feet_obj_id));
        byte_builder_append(buffer, (unsigned char *) &back_obj_id, sizeof(back_obj_id));
        byte_builder_append(buffer, (unsigned char *) &lr_hand_obj_id, sizeof(lr_hand_obj_id));
        byte_builder_append(buffer, (unsigned char *) &hair_obj_id, sizeof(hair_obj_id));

        byte_builder_append(buffer, (unsigned char *) &under, sizeof(under));
        byte_builder_append(buffer, (unsigned char *) &r_ear, sizeof(r_ear));
        byte_builder_append(buffer, (unsigned char *) &l_ear, sizeof(l_ear));
        byte_builder_append(buffer, (unsigned char *) &neck, sizeof(neck));
        byte_builder_append(buffer, (unsigned char *) &r_finger, sizeof(r_finger));
        byte_builder_append(buffer, (unsigned char *) &l_finger, sizeof(l_finger));
        byte_builder_append(buffer, (unsigned char *) &head, sizeof(head));
        byte_builder_append(buffer, (unsigned char *) &r_hand, sizeof(r_hand));
        byte_builder_append(buffer, (unsigned char *) &l_hand, sizeof(l_hand));
        byte_builder_append(buffer, (unsigned char *) &gloves, sizeof(gloves));
        byte_builder_append(buffer, (unsigned char *) &chest, sizeof(chest));
        byte_builder_append(buffer, (unsigned char *) &legs, sizeof(legs));
        byte_builder_append(buffer, (unsigned char *) &feet, sizeof(feet));
        byte_builder_append(buffer, (unsigned char *) &back, sizeof(back));
        byte_builder_append(buffer, (unsigned char *) &lr_hand, sizeof(lr_hand));
        byte_builder_append(buffer, (unsigned char *) &hair, sizeof(hair));

        byte_builder_append(
                buffer,
                (unsigned char *) &player->hair_style_id,
                sizeof(player->hair_style_id)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->hair_color_id,
                sizeof(player->hair_color_id)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->face,
                sizeof(player->face)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &player->character.hp,
                sizeof(player->character.hp)
        );

        byte_builder_append(
                buffer, 
                (unsigned char *) &player->character.mp,
                sizeof(&player->character.mp)
        );

        byte_builder_append(buffer, (unsigned char *) &delete_days, sizeof(delete_days));

        byte_builder_append(
                buffer,
                (unsigned char *) &player->class_id,
                sizeof(player->class_id)
        );

        byte_builder_append(buffer, (unsigned char *) &auto_select, sizeof(auto_select));
        byte_builder_append(buffer, (unsigned char *) &enchant_effect, sizeof(enchant_effect));
}

l2_packet* game_request_auth_login_response(struct GameClient* client)
{
        assert(client);

        l2_packet_type type = 0x13;

        struct L2SessionKey* key = game_client_session(client);

        int play_ok_1 = key->playOK1;

        size_t max_chars = 5;
        struct Player** chars = game_client_alloc_temp_mem(
                client,
                sizeof(struct Player *) * max_chars
        );
        assert(chars);

        int char_count = 0;
        byte_builder* buffer;

        for (size_t i = 0; i < max_chars; i++) {
                chars[i] = game_client_alloc_temp_mem(
                        client,
                        sizeof(struct Player)
                );
                assert(chars[i]);
        }

        char_count = game_service_character_list(chars, max_chars);
        buffer = game_client_byte_builder(
                client,
                (size_t) (300 * (char_count + 1))
        );
        assert(buffer);

        byte_builder_append(
                buffer,
                (unsigned char *) &char_count,
                sizeof(char_count)
        );

        for (int i = 0; i < char_count; i++) {
                game_packet_char_list_add_char_to_buffer(
                        buffer,
                        play_ok_1,
                        chars[i]
                );
        }

        return game_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
