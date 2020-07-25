#include <string.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <core/l2_string.h>
#include "../../storage/conn.h"
#include "../../storage/character.h"
#include "../../client.h"
#include "../../entity/pc.h"
#include "../../packet_builder.h"
#include "response.h"

static void char_to_buffer(byte_builder *buffer, int play_ok_1, struct Pc *player)
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
                name,
                l2_string_calculate_space_from_char(strlen(player->character.name) + 1)
        );

        byte_builder_append(buffer, &player->character.id, sizeof(player->character.id));

        byte_builder_append(buffer, login, sizeof(login));

        byte_builder_append(buffer, &play_ok_1, sizeof(play_ok_1));

        byte_builder_append(buffer, &clan_id, sizeof(clan_id));

        byte_builder_append(buffer, &empty_space, sizeof(empty_space));

        byte_builder_append(buffer, &player->character.sex, sizeof(player->character.sex));
        byte_builder_append(buffer, &player->race_id, sizeof(player->race_id));
        byte_builder_append(buffer, &player->class_id, sizeof(player->class_id));
        byte_builder_append(buffer, &player->active, sizeof(player->active));

        byte_builder_append(
                buffer,
                (unsigned char * ) &empty_space,
                sizeof(empty_space)
                // &character->current_location.x,
                // sizeof(character->current_location.x)
        );

        byte_builder_append(
                buffer,
                &empty_space,
                sizeof(empty_space)
                // &character->current_location.y,
                // sizeof(character->current_location.y)
        );

        byte_builder_append(
                buffer,
                &empty_space,
                sizeof(empty_space)
                // &character->current_location.z,
                // sizeof(character->current_location.z)
        );

        byte_builder_append(
                buffer,
                &player->character.current_hp,
                sizeof(player->character.current_hp)
        );
        
        byte_builder_append(
                buffer,
                &player->character.current_mp,
                sizeof(player->character.current_mp)
        );

        byte_builder_append(buffer, &sp, sizeof(sp));
        byte_builder_append(buffer, &exp, sizeof(exp));

        byte_builder_append(buffer, &player->character.level, sizeof(player->character.level));

        byte_builder_append(buffer, &karma, sizeof(karma));

        byte_builder_append(buffer, &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, &empty_space, sizeof(empty_space));

        byte_builder_append(buffer, &under_obj_id, sizeof(under_obj_id));
        byte_builder_append(buffer, &r_ear_obj_id, sizeof(r_ear_obj_id));
        byte_builder_append(buffer, &l_ear_obj_id, sizeof(l_ear_obj_id));
        byte_builder_append(buffer, &neck_obj_id, sizeof(neck_obj_id));
        byte_builder_append(buffer, &r_finger_obj_id, sizeof(r_finger_obj_id));
        byte_builder_append(buffer, &l_finger_obj_id, sizeof(l_finger_obj_id));
        byte_builder_append(buffer, &head_obj_id, sizeof(head_obj_id));
        byte_builder_append(buffer, &r_hand_obj_id, sizeof(r_hand_obj_id));
        byte_builder_append(buffer, &l_hand_obj_id, sizeof(l_hand_obj_id));
        byte_builder_append(buffer, &gloves_obj_id, sizeof(gloves_obj_id));
        byte_builder_append(buffer, &chest_obj_id, sizeof(chest_obj_id));
        byte_builder_append(buffer, &legs_obj_id, sizeof(legs_obj_id));
        byte_builder_append(buffer, &feet_obj_id, sizeof(feet_obj_id));
        byte_builder_append(buffer, &back_obj_id, sizeof(back_obj_id));
        byte_builder_append(buffer, &lr_hand_obj_id, sizeof(lr_hand_obj_id));
        byte_builder_append(buffer, &hair_obj_id, sizeof(hair_obj_id));

        byte_builder_append(buffer, &under, sizeof(under));
        byte_builder_append(buffer, &r_ear, sizeof(r_ear));
        byte_builder_append(buffer, &l_ear, sizeof(l_ear));
        byte_builder_append(buffer, &neck, sizeof(neck));
        byte_builder_append(buffer, &r_finger, sizeof(r_finger));
        byte_builder_append(buffer, &l_finger, sizeof(l_finger));
        byte_builder_append(buffer, &head, sizeof(head));
        byte_builder_append(buffer, &r_hand, sizeof(r_hand));
        byte_builder_append(buffer, &l_hand, sizeof(l_hand));
        byte_builder_append(buffer, &gloves, sizeof(gloves));
        byte_builder_append(buffer, &chest, sizeof(chest));
        byte_builder_append(buffer, &legs, sizeof(legs));
        byte_builder_append(buffer, &feet, sizeof(feet));
        byte_builder_append(buffer, &back, sizeof(back));
        byte_builder_append(buffer, &lr_hand, sizeof(lr_hand));
        byte_builder_append(buffer, &hair, sizeof(hair));

        byte_builder_append(buffer, &player->hair_style_id, sizeof(player->hair_style_id));
        byte_builder_append(buffer, &player->hair_color_id, sizeof(player->hair_color_id));
        byte_builder_append(buffer, &player->face, sizeof(player->face));

        byte_builder_append(buffer, &player->character.hp, sizeof(player->character.hp));
        byte_builder_append(buffer, &player->character.mp, sizeof(&player->character.mp));

        byte_builder_append(buffer, &delete_days, sizeof(delete_days));

        byte_builder_append(buffer, &player->class_id, sizeof(player->class_id));

        byte_builder_append(buffer, &auto_select, sizeof(auto_select));
        byte_builder_append(buffer, &enchant_effect, sizeof(enchant_effect));
}

l2_packet *auth_login_response(struct Client *client)
{
        l2_packet_type type = 0x13;
        l2_packet *response;

        struct L2SessionKey *key = client_session(client);

        conn_handler *conn = client_alloc_mem(client, conn_handler_size());
        struct Pc **characters;
        int characters_count = 1;

        size_t buf_size = byte_builder_calculate_size(1024);
        byte_builder *buf = client_alloc_mem(client, buf_size);
        byte_builder *builder = byte_builder_init(buf, buf_size);

        // conn_open(conn);
        characters = storage_characters_all(conn, client);
        // conn_close(conn);

        log_info("Found %d characters for %s", characters_count, key->login_name);

        byte_builder_append(builder, &characters_count, sizeof(characters_count));

        for (int i = 0; i < characters_count; i++) {
                char_to_buffer(builder, key->playOK1, characters[i]);
                client_free_mem(client, characters[i]);
        }

        response = packet_builder_new(client, type, builder, byte_builder_length(builder));

        client_free_mem(client, conn);
        client_free_mem(client, buf);
        client_free_mem(client, characters);
        client_free_mem(client, key);

        return response;
}
