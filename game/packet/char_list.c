#ifndef L2AUTH_LOGIN_GAME_PACKET_CHAR_LIST_C
#define L2AUTH_LOGIN_GAME_PACKET_CHAR_LIST_C

#include <assert.h>
#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <core/byte_builder.h>
#include <login/dto/session_key.h>
#include <game/dto/char.h>
#include <game/packet/char_list.h>

l2_packet* game_packet_char_list(struct L2Client* client)
{
        assert(client);

        l2_packet_type type = 0x13;

        struct LoginDtoSessionKey* key = l2_client_session(client);
        int play_ok_1 = key->playOK1;

        int char_count = 0x01;

        char name[] = { 'r', 0, 'u', 0, 'k', 0, 'e', 0, 0, 0 };
        int char_id = 0x00;
        char login[] = { 'r', 0, 'u', 0, 'k', 0, 'e', 0, 0, 0 };

        int clan_id = 0x00;

        int empty_space = 0x00;

        int sex = 0x00;
        int race = 0x00;
        int class_id = 0x00;

        int active = 0x01;

        int x = 0x00;
        int y = 0x00;
        int z = 0x00;

        double current_hp = 100.50;
        double current_mp = 50.3;

        int sp = 5;
        int exp = 10;
        int level = 10;
        int karma = 1;

        unsigned char empty_block[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

        int under_obj_id = 0x01;
        int r_ear_obj_id = 0x01;
        int l_ear_obj_id = 0x01;
        int neck_obj_id = 0x01;
        int r_finger_obj_id = 0x01;
        int l_finger_obj_id = 0x01;
        int head_obj_id = 0x01;
        int r_hand_obj_id = 0x01;
        int l_hand_obj_id = 0x01;
        int gloves_obj_id = 0x01;
        int chest_obj_id = 0x01;
        int legs_obj_id = 0x01;
        int feet_obj_id = 0x01;
        int back_obj_id = 0x01;
        int lr_hand_obj_id = 0x01;
        int hair_obj_id = 0x01;

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

        int hair_style = 0x01;
        int hair_color = 0x01;
        int face = 0x01;

        double max_hp = 5000.0;
        double max_mp = 100.0;

        int delete_days = 0x00;

        int auto_select = 0x00;
        int enchant_effect = 0x00;

        byte_builder* buffer = l2_client_byte_builder(client, 300);

        l2_packet *packet;

        byte_builder_append(buffer, &char_count, sizeof(char_count));
        byte_builder_append(buffer, name, sizeof(name));
        byte_builder_append(buffer, &char_id, sizeof(char_id));
        byte_builder_append(buffer, login, sizeof(login));
        byte_builder_append(buffer, &play_ok_1, sizeof(play_ok_1));
        byte_builder_append(buffer, &clan_id, sizeof(clan_id));
        byte_builder_append(buffer, &empty_space, sizeof(empty_space));
        byte_builder_append(buffer, &sex, sizeof(sex));
        byte_builder_append(buffer, &race, sizeof(race));
        byte_builder_append(buffer, &class_id, sizeof(class_id));
        byte_builder_append(buffer, &active, sizeof(active));
        byte_builder_append(buffer, &x, sizeof(x));
        byte_builder_append(buffer, &y, sizeof(y));
        byte_builder_append(buffer, &z, sizeof(z));
        byte_builder_append(buffer, &current_hp, sizeof(current_hp));
        byte_builder_append(buffer, &current_mp, sizeof(current_mp));
        byte_builder_append(buffer, &sp, sizeof(sp));
        byte_builder_append(buffer, &exp, sizeof(exp));
        byte_builder_append(buffer, &level, sizeof(level));
        byte_builder_append(buffer, &karma, sizeof(karma));
        byte_builder_append(buffer, empty_block, sizeof(empty_block));
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
        byte_builder_append(buffer, &hair_style, sizeof(hair_style));
        byte_builder_append(buffer, &hair_color, sizeof(hair_color));
        byte_builder_append(buffer, &face, sizeof(face));
        byte_builder_append(buffer, &max_hp, sizeof(max_hp));
        byte_builder_append(buffer, &max_mp, sizeof(max_mp));
        byte_builder_append(buffer, &delete_days, sizeof(delete_days));
        byte_builder_append(buffer, &class_id, sizeof(class_id));
        byte_builder_append(buffer, &auto_select, sizeof(auto_select));
        byte_builder_append(buffer, &enchant_effect, sizeof(enchant_effect));

        return l2_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}

#endif
