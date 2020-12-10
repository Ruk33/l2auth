#include "../headers.h"
#include "../character.h"
#include "auth_login.h"

static byte *write_char_to_buf(byte *p, struct ServerPacketAuthLoginCharacter *character)
{
        assert(p);
        assert(character);

        BYTE_WRITE(p, character->name, l2_string_bytes(character->name));
        BYTE_WRITE_VAL(p, character->id);
        BYTE_WRITE(p, character->name_copy, l2_string_bytes(character->name_copy));
        BYTE_WRITE_VAL(p, character->playOK1);
        BYTE_WRITE_VAL(p, character->clan_id);
        BYTE_WRITE_VAL(p, character->empty[0]);
        BYTE_WRITE_VAL(p, character->sex);
        BYTE_WRITE_VAL(p, character->race_id);
        BYTE_WRITE_VAL(p, character->class_id);
        BYTE_WRITE_VAL(p, character->active);
        BYTE_WRITE_VAL(p, character->x);
        BYTE_WRITE_VAL(p, character->y);
        BYTE_WRITE_VAL(p, character->z);
        BYTE_WRITE_VAL(p, character->hp);
        BYTE_WRITE_VAL(p, character->mp);
        BYTE_WRITE_VAL(p, character->sp);
        BYTE_WRITE_VAL(p, character->exp);
        BYTE_WRITE_VAL(p, character->level);
        BYTE_WRITE_VAL(p, character->karma);

        BYTE_WRITE(p, character->empty, 9 * sizeof(character->empty[0]));

        BYTE_WRITE_VAL(p, character->under_obj_id);
        BYTE_WRITE_VAL(p, character->r_ear_obj_id);
        BYTE_WRITE_VAL(p, character->l_ear_obj_id);
        BYTE_WRITE_VAL(p, character->neck_obj_id);
        BYTE_WRITE_VAL(p, character->r_finger_obj_id);
        BYTE_WRITE_VAL(p, character->l_finger_obj_id);
        BYTE_WRITE_VAL(p, character->head_obj_id);
        BYTE_WRITE_VAL(p, character->r_hand_obj_id);
        BYTE_WRITE_VAL(p, character->l_hand_obj_id);
        BYTE_WRITE_VAL(p, character->gloves_obj_id);
        BYTE_WRITE_VAL(p, character->chest_obj_id);
        BYTE_WRITE_VAL(p, character->legs_obj_id);
        BYTE_WRITE_VAL(p, character->feet_obj_id);
        BYTE_WRITE_VAL(p, character->back_obj_id);
        BYTE_WRITE_VAL(p, character->lr_hand_obj_id);
        BYTE_WRITE_VAL(p, character->hair_obj_id);

        BYTE_WRITE_VAL(p, character->under);
        BYTE_WRITE_VAL(p, character->r_ear);
        BYTE_WRITE_VAL(p, character->l_ear);
        BYTE_WRITE_VAL(p, character->neck);
        BYTE_WRITE_VAL(p, character->r_finger);
        BYTE_WRITE_VAL(p, character->l_finger);
        BYTE_WRITE_VAL(p, character->head);
        BYTE_WRITE_VAL(p, character->r_hand);
        BYTE_WRITE_VAL(p, character->l_hand);
        BYTE_WRITE_VAL(p, character->gloves);
        BYTE_WRITE_VAL(p, character->chest);
        BYTE_WRITE_VAL(p, character->legs);
        BYTE_WRITE_VAL(p, character->feet);
        BYTE_WRITE_VAL(p, character->back);
        BYTE_WRITE_VAL(p, character->lr_hand);
        BYTE_WRITE_VAL(p, character->hair);

        BYTE_WRITE_VAL(p, character->hair_style_id);
        BYTE_WRITE_VAL(p, character->hair_color_id);
        BYTE_WRITE_VAL(p, character->face);
        BYTE_WRITE_VAL(p, character->max_hp);
        BYTE_WRITE_VAL(p, character->max_mp);
        BYTE_WRITE_VAL(p, character->delete_days);
        BYTE_WRITE_VAL(p, character->class_id);
        BYTE_WRITE_VAL(p, character->auto_select);
        BYTE_WRITE_VAL(p, character->enchant_effect);

        return p;
}

void server_packet_auth_login(packet *dest, int playOK1, struct List *characters)
{
        assert(dest);

        packet_type type = 0x13;

        struct ListEntry *iterator = NULL;
        character_t *character = NULL;
        int character_count = 0;

        struct ServerPacketAuthLogin auth_login_packet = {0};
        byte buf[SERVER_PACKET_AUTH_LOGIN_SIZE] = {0};
        byte *p = buf;

        if (characters)
        {
                iterator = list_get_iterator(characters);
        }

        while (iterator)
        {
                character_count += 1;
                iterator = list_get_next(iterator);
        }

        auth_login_packet.character_count = character_count;
        BYTE_WRITE_VAL(p, auth_login_packet.character_count);

        if (characters)
        {
                iterator = list_get_iterator(characters);
        }

        for (int i = 0; i < character_count; i++)
        {
                character = list_get_value(iterator);

                l2_string_from_char(
                        auth_login_packet.characters[i].name,
                        character->name,
                        strlen(character->name) + 1
                );
                l2_string_from_char(
                        auth_login_packet.characters[i].name_copy,
                        character->name,
                        strlen(character->name) + 1
                );

                auth_login_packet.characters[i].id = character->id;
                auth_login_packet.characters[i].playOK1 = playOK1;
                auth_login_packet.characters[i].active = character->active;
                auth_login_packet.characters[i].level = character->level;
                auth_login_packet.characters[i].hp = character->hp;
                auth_login_packet.characters[i].mp = character->mp;
                auth_login_packet.characters[i].max_hp = character->max_hp;
                auth_login_packet.characters[i].max_mp = character->max_mp;
                auth_login_packet.characters[i].clan_id = character->clan_id;
                auth_login_packet.characters[i].sex = character->sex;
                auth_login_packet.characters[i].race_id = character->race_id;
                auth_login_packet.characters[i].class_id = character->class_id;
                auth_login_packet.characters[i].x = character->x;
                auth_login_packet.characters[i].y = character->y;
                auth_login_packet.characters[i].z = character->z;
                auth_login_packet.characters[i].sp = character->sp;
                auth_login_packet.characters[i].exp = character->exp;
                auth_login_packet.characters[i].level = character->level;
                auth_login_packet.characters[i].karma = character->karma;

                p = write_char_to_buf(p, &auth_login_packet.characters[i]);

                iterator = list_get_next(iterator);
        }

        packet_build(dest, type, buf, (size_t) (p - buf));
}
