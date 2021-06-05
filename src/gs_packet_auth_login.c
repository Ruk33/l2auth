#include "include/util.h"
#include "include/packet.h"
#include "include/gs_character.h"
#include "include/gs_packet_auth_login.h"

void gs_packet_auth_login_add_character(
        gs_packet_auth_login_t *dest,
        gs_character_t *src)
{
        gs_packet_auth_login_char_t *character = 0;

        size_t name_size     = 0;
        size_t name_cpy_size = 0;

        character = &dest->characters[dest->count];

        name_size     = sizeof(character->name);
        name_cpy_size = sizeof(character->name_copy);

        l2_string_from_char(character->name, src->name, name_size);
        l2_string_from_char(character->name_copy, src->name, name_cpy_size);

        character->active        = 1;
        character->class_id      = src->_class;
        character->class_id_copy = src->_class;
        character->face          = src->face;
        character->hair_color_id = src->hair_color;
        character->hair_style_id = src->hair_style;
        character->hp            = 400;
        character->id            = 42;
        character->level         = 1;
        character->max_hp        = 400;
        character->max_mp        = 400;
        character->mp            = 400;
        character->sex           = src->sex;

        dest->count += 1;
}

void gs_packet_auth_login_pack(packet_t *dest, gs_packet_auth_login_t *src)
{
        byte_t type = 0;

        gs_packet_auth_login_char_t *character = 0;

        size_t name_size = 0;

        type = 0x13;

        packet_append_val(dest, type);
        packet_append_val(dest, src->count);

        for (int i = 0; i < src->count; i += 1) {
                character = &src->characters[i];
                name_size = l2_string_bytes(character->name);

                packet_append_n(dest, (byte_t *) character->name, name_size);
                packet_append_val(dest, character->id);
                packet_append_n(
                        dest, (byte_t *) character->name_copy, name_size);
                packet_append_val(dest, character->playOK1);
                packet_append_val(dest, character->clan_id);
                packet_append_val(dest, character->empty[0]);
                packet_append_val(dest, character->sex);
                packet_append_val(dest, character->race_id);
                packet_append_val(dest, character->class_id);
                packet_append_val(dest, character->active);
                packet_append_val(dest, character->x);
                packet_append_val(dest, character->y);
                packet_append_val(dest, character->z);
                packet_append_val(dest, character->hp);
                packet_append_val(dest, character->mp);
                packet_append_val(dest, character->sp);
                packet_append_val(dest, character->exp);
                packet_append_val(dest, character->level);
                packet_append_val(dest, character->karma);

                packet_append_n(
                        dest,
                        (byte_t *) character->empty,
                        9 * sizeof(character->empty[0]));

                packet_append_val(dest, character->under_obj_id);
                packet_append_val(dest, character->r_ear_obj_id);
                packet_append_val(dest, character->l_ear_obj_id);
                packet_append_val(dest, character->neck_obj_id);
                packet_append_val(dest, character->r_finger_obj_id);
                packet_append_val(dest, character->l_finger_obj_id);
                packet_append_val(dest, character->head_obj_id);
                packet_append_val(dest, character->r_hand_obj_id);
                packet_append_val(dest, character->l_hand_obj_id);
                packet_append_val(dest, character->gloves_obj_id);
                packet_append_val(dest, character->chest_obj_id);
                packet_append_val(dest, character->legs_obj_id);
                packet_append_val(dest, character->feet_obj_id);
                packet_append_val(dest, character->back_obj_id);
                packet_append_val(dest, character->lr_hand_obj_id);
                packet_append_val(dest, character->hair_obj_id);

                packet_append_val(dest, character->under);
                packet_append_val(dest, character->r_ear);
                packet_append_val(dest, character->l_ear);
                packet_append_val(dest, character->neck);
                packet_append_val(dest, character->r_finger);
                packet_append_val(dest, character->l_finger);
                packet_append_val(dest, character->head);
                packet_append_val(dest, character->r_hand);
                packet_append_val(dest, character->l_hand);
                packet_append_val(dest, character->gloves);
                packet_append_val(dest, character->chest);
                packet_append_val(dest, character->legs);
                packet_append_val(dest, character->feet);
                packet_append_val(dest, character->back);
                packet_append_val(dest, character->lr_hand);
                packet_append_val(dest, character->hair);

                packet_append_val(dest, character->hair_style_id);
                packet_append_val(dest, character->hair_color_id);
                packet_append_val(dest, character->face);
                packet_append_val(dest, character->max_hp);
                packet_append_val(dest, character->max_mp);
                packet_append_val(dest, character->delete_days);
                packet_append_val(dest, character->class_id);
                packet_append_val(dest, character->auto_select);
                packet_append_val(dest, character->enchant_effect);
        }
}
