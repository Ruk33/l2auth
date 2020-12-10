#include "../headers.h"
#include "select_character.h"

void server_packet_select_character(packet *dest, character_t *character, int playOK1)
{
        assert(dest);
        assert(character);

        packet_type type = 0x15;

        byte buf[SERVER_PACKET_SELECT_CHARACTER_SIZE] = {0};
        byte *p = buf;

        struct ServerPacketSelectCharacter select_character_packet = {0};

        l2_string_from_char(
                select_character_packet.name,
                character->name,
                strlen(character->name) + 1
        );

        l2_string_from_char(
                select_character_packet.title,
                character->title,
                strlen(character->title) + 1
        );

        select_character_packet.id = character->id;
        select_character_packet.playOK1 = playOK1;
        select_character_packet.clan_id = character->clan_id;
        select_character_packet.sex = character->sex;
        select_character_packet.race_id = character->race_id;
        select_character_packet.class_id = character->class_id;
        select_character_packet.active = character->active;
        select_character_packet.x = character->x;
        select_character_packet.y = character->y;
        select_character_packet.z = character->z;
        select_character_packet.hp = character->hp;
        select_character_packet.mp = character->mp;
        select_character_packet.sp = character->sp;
        select_character_packet.exp = character->exp;
        select_character_packet.level = character->level;
        select_character_packet._int = character->_int;
        select_character_packet.str = character->str;
        select_character_packet.con = character->con;
        select_character_packet.men = character->men;
        select_character_packet.dex = character->dex;
        select_character_packet.wit = character->wit;
        select_character_packet.game_time = 42;

        BYTE_WRITE(
                p,
                select_character_packet.name,
                l2_string_bytes(select_character_packet.name)
        );
        BYTE_WRITE_VAL(p, select_character_packet.id);
        BYTE_WRITE(
                p,
                select_character_packet.title,
                l2_string_bytes(select_character_packet.title)
        );
        BYTE_WRITE_VAL(p, select_character_packet.playOK1);
        BYTE_WRITE_VAL(p, select_character_packet.clan_id);
        BYTE_WRITE_VAL(p, select_character_packet.empty[0]);
        BYTE_WRITE_VAL(p, select_character_packet.sex);
        BYTE_WRITE_VAL(p, select_character_packet.race_id);
        BYTE_WRITE_VAL(p, select_character_packet.class_id);
        BYTE_WRITE_VAL(p, select_character_packet.active);
        BYTE_WRITE_VAL(p, select_character_packet.x);
        BYTE_WRITE_VAL(p, select_character_packet.y);
        BYTE_WRITE_VAL(p, select_character_packet.z);
        BYTE_WRITE_VAL(p, select_character_packet.hp);
        BYTE_WRITE_VAL(p, select_character_packet.mp);
        BYTE_WRITE_VAL(p, select_character_packet.sp);
        BYTE_WRITE_VAL(p, select_character_packet.exp);
        BYTE_WRITE_VAL(p, select_character_packet.level);
        BYTE_WRITE_VAL(p, select_character_packet.empty[0]);
        BYTE_WRITE_VAL(p, select_character_packet.empty[0]);
        BYTE_WRITE_VAL(p, select_character_packet._int);
        BYTE_WRITE_VAL(p, select_character_packet.str);
        BYTE_WRITE_VAL(p, select_character_packet.con);
        BYTE_WRITE_VAL(p, select_character_packet.men);
        BYTE_WRITE_VAL(p, select_character_packet.dex);
        BYTE_WRITE_VAL(p, select_character_packet.wit);
        BYTE_WRITE(p, select_character_packet.empty, sizeof(select_character_packet.empty[0]) * 36);
        BYTE_WRITE_VAL(p, select_character_packet.game_time);
        BYTE_WRITE(p, select_character_packet.empty, sizeof(select_character_packet.empty[0]) * 15);

        packet_build(dest, type, buf, (size_t) (p - buf));
}
