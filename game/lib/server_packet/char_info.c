#include "../headers.h"
#include "../character.h"
#include "char_info.h"

void server_packet_char_info(packet *dest, character_t *character, int heading)
{
        assert(dest);
        assert(character);

        packet_type type = 0x03;

        unsigned char buf[SERVER_PACKET_CHAR_INFO_SIZE] = {0};
        unsigned char *p = buf;

        struct ServerPacketCharInfo char_info_packet = {0};

        char_info_packet.x = character->x;
        char_info_packet.y = character->y;
        char_info_packet.z = character->z;
        char_info_packet.heading = heading;
        char_info_packet.race_id = character->race_id;
        char_info_packet.sex = character->sex;
        char_info_packet.class_id = character->class_id;

        l2_string_from_char(
                char_info_packet.char_name,
                character->name,
                strlen(character->name) + 1
        );

        l2_string_from_char(
                char_info_packet.title,
                character->title,
                strlen(character->title) + 1
        );

        BYTE_WRITE_VAL(p, char_info_packet.x);
        BYTE_WRITE_VAL(p, char_info_packet.y);
        BYTE_WRITE_VAL(p, char_info_packet.z);
        BYTE_WRITE_VAL(p, heading);
        BYTE_WRITE_VAL(p, char_info_packet.char_id);
        BYTE_WRITE(p, char_info_packet.char_name, l2_string_bytes(char_info_packet.char_name));
        BYTE_WRITE_VAL(p, char_info_packet.race_id);
        BYTE_WRITE_VAL(p, char_info_packet.sex);
        BYTE_WRITE_VAL(p, char_info_packet.class_id);
        BYTE_WRITE_VAL(p, char_info_packet.empty[0]);
        BYTE_WRITE(p, char_info_packet.paperdoll, sizeof(char_info_packet.paperdoll));
        BYTE_WRITE_VAL(p, char_info_packet.pvp_flag);
        BYTE_WRITE_VAL(p, char_info_packet.karma);
        BYTE_WRITE_VAL(p, char_info_packet.m_attack_speed);
        BYTE_WRITE_VAL(p, char_info_packet.p_attack_speed);
        BYTE_WRITE_VAL(p, char_info_packet.pvp_flag_copy);
        BYTE_WRITE_VAL(p, char_info_packet.karma_copy);
        BYTE_WRITE_VAL(p, char_info_packet.run_speed);
        BYTE_WRITE_VAL(p, char_info_packet.walk_speed);
        BYTE_WRITE_VAL(p, char_info_packet.swim_run_speed);
        BYTE_WRITE_VAL(p, char_info_packet.swim_walk_speed);
        BYTE_WRITE_VAL(p, char_info_packet.fly_run_speed);
        BYTE_WRITE_VAL(p, char_info_packet.fly_walk_speed);
        BYTE_WRITE_VAL(p, char_info_packet.fly_run_speed_copy);
        BYTE_WRITE_VAL(p, char_info_packet.fly_walk_speed_copy);
        BYTE_WRITE_VAL(p, char_info_packet.movement_multiplier);
        BYTE_WRITE_VAL(p, char_info_packet.attack_speed_multiplier);
        BYTE_WRITE_VAL(p, char_info_packet.collision_radius);
        BYTE_WRITE_VAL(p, char_info_packet.collision_height);
        BYTE_WRITE_VAL(p, char_info_packet.hair_style_id);
        BYTE_WRITE_VAL(p, char_info_packet.hair_color_id);
        BYTE_WRITE_VAL(p, char_info_packet.face);
        BYTE_WRITE(p, char_info_packet.title, l2_string_bytes(char_info_packet.title));
        BYTE_WRITE_VAL(p, char_info_packet.clan_id);
        BYTE_WRITE_VAL(p, char_info_packet.clan_crest_id);
        BYTE_WRITE_VAL(p, char_info_packet.ally_id);
        BYTE_WRITE_VAL(p, char_info_packet.ally_crest_id);
        BYTE_WRITE_VAL(p, char_info_packet.empty[0]);
        BYTE_WRITE_VAL(p, char_info_packet.standing);
        BYTE_WRITE_VAL(p, char_info_packet.running);
        BYTE_WRITE_VAL(p, char_info_packet.in_combat);
        BYTE_WRITE_VAL(p, char_info_packet.alike_dead);
        BYTE_WRITE_VAL(p, char_info_packet.invisible);
        BYTE_WRITE_VAL(p, char_info_packet.mount_type);
        BYTE_WRITE_VAL(p, char_info_packet.private_store_type);
        BYTE_WRITE_VAL(p, char_info_packet.cubics);
        BYTE_WRITE_VAL(p, char_info_packet.find_party_member);
        BYTE_WRITE_VAL(p, char_info_packet.abnormal_effect);
        BYTE_WRITE_VAL(p, char_info_packet.empty_char[0]);
        BYTE_WRITE_VAL(p, char_info_packet.recommendations_left);
        BYTE_WRITE_VAL(p, char_info_packet.empty[0]);
        BYTE_WRITE_VAL(p, char_info_packet.empty[1]);
        BYTE_WRITE_VAL(p, char_info_packet.empty[2]);
        BYTE_WRITE_VAL(p, char_info_packet.mounted);
        BYTE_WRITE_VAL(p, char_info_packet.empty_char[0]);
        BYTE_WRITE_VAL(p, char_info_packet.clan_crest_large_id);
        BYTE_WRITE_VAL(p, char_info_packet.hero_symbol);
        BYTE_WRITE_VAL(p, char_info_packet.hero_aura);
        BYTE_WRITE_VAL(p, char_info_packet.fishing);
        BYTE_WRITE_VAL(p, char_info_packet.empty[0]);
        BYTE_WRITE_VAL(p, char_info_packet.empty[1]);
        BYTE_WRITE_VAL(p, char_info_packet.empty[2]);
        BYTE_WRITE_VAL(p, char_info_packet.name_color);

        packet_build(dest, type, buf, (size_t) (p - buf));
}
