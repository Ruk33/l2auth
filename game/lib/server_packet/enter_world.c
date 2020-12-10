#include "../headers.h"
#include "../character.h"
#include "enter_world.h"

void server_packet_enter_world(packet *dest, character_t *character)
{
        assert(dest);
        assert(character);

        packet_type type = 0x04;

        byte buf[SERVER_PACKET_ENTER_WORLD_SIZE] = {0};
        byte *p = buf;

        struct ServerPacketEnterWorld enter_world_packet = {0};

        l2_string_from_char(
                enter_world_packet.name,
                character->name,
                strlen(character->name) + 1
        );

        l2_string_from_char(
                enter_world_packet.title,
                character->title,
                strlen(character->title) + 1
        );

        enter_world_packet.unknown = 0x28;
        enter_world_packet.run_speed = 120;
        enter_world_packet.walk_speed = 100;
        enter_world_packet.p_attack_speed = 20;
        enter_world_packet.p_attack_speed_copy  = 20;
        enter_world_packet.movement_speed_multiplier = 1;
        enter_world_packet.attack_speed_multiplier = 1;
        enter_world_packet.collision_radius = 9;
        enter_world_packet.collision_height = 23;
        enter_world_packet.name_color = 0xFFFFFF;
        enter_world_packet.max_load = 42;

        enter_world_packet.x = character->x;
        enter_world_packet.y = character->y;
        enter_world_packet.z = character->z;
        enter_world_packet.id = character->id;
        enter_world_packet.race_id = character->race_id;
        enter_world_packet.sex = character->sex;
        enter_world_packet.class_id = character->class_id;
        enter_world_packet.class_id_copy = character->class_id;
        enter_world_packet.level = character->level;
        enter_world_packet.exp = character->exp;
        enter_world_packet.str = character->str;
        enter_world_packet.dex = character->dex;
        enter_world_packet.con = character->con;
        enter_world_packet._int = character->_int;
        enter_world_packet.wit = character->wit;
        enter_world_packet.men = character->men;
        enter_world_packet.hp = (int) character->hp;
        enter_world_packet.mp = (int) character->mp;
        enter_world_packet.max_hp = (int) character->max_hp;
        enter_world_packet.max_mp = (int) character->max_mp;
        enter_world_packet.cp = character->cp;
        enter_world_packet.max_cp = character->max_cp;
        enter_world_packet.sp = character->sp;
        enter_world_packet.exp = character->exp;
        enter_world_packet.sp = character->sp;
        enter_world_packet.p_attack = character->p_attack;
        enter_world_packet.m_attack = character->m_attack;
        enter_world_packet.p_def = character->p_def;
        enter_world_packet.m_def = character->m_def;
        enter_world_packet.evasion_rate = character->evasion_rate;
        enter_world_packet.critical_hit  = character->critical_hit;

        BYTE_WRITE_VAL(p, enter_world_packet.x);
        BYTE_WRITE_VAL(p, enter_world_packet.y);
        BYTE_WRITE_VAL(p, enter_world_packet.z);
        BYTE_WRITE_VAL(p, enter_world_packet.heading);
        BYTE_WRITE_VAL(p, enter_world_packet.id);
        BYTE_WRITE(p, enter_world_packet.name, l2_string_bytes(enter_world_packet.name));
        BYTE_WRITE_VAL(p, enter_world_packet.race_id);
        BYTE_WRITE_VAL(p, enter_world_packet.sex);
        BYTE_WRITE_VAL(p, enter_world_packet.class_id);
        BYTE_WRITE_VAL(p, enter_world_packet.level);
        BYTE_WRITE_VAL(p, enter_world_packet.exp);
        BYTE_WRITE_VAL(p, enter_world_packet.str);
        BYTE_WRITE_VAL(p, enter_world_packet.dex);
        BYTE_WRITE_VAL(p, enter_world_packet.con);
        BYTE_WRITE_VAL(p, enter_world_packet._int);
        BYTE_WRITE_VAL(p, enter_world_packet.wit);
        BYTE_WRITE_VAL(p, enter_world_packet.men);
        BYTE_WRITE_VAL(p, enter_world_packet.max_hp);
        BYTE_WRITE_VAL(p, enter_world_packet.hp);
        BYTE_WRITE_VAL(p, enter_world_packet.max_mp);
        BYTE_WRITE_VAL(p, enter_world_packet.mp);
        BYTE_WRITE_VAL(p, enter_world_packet.sp);
        BYTE_WRITE_VAL(p, enter_world_packet.current_load);
        BYTE_WRITE_VAL(p, enter_world_packet.max_load);
        BYTE_WRITE_VAL(p, enter_world_packet.unknown);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_under);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_r_ear);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_l_ear);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_neck);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_r_finger);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_l_finger);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_head);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_r_hand);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_l_hand);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_gloves);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_chest);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_legs);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_feet);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_back);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_lr_hand);
        BYTE_WRITE_VAL(p, enter_world_packet.paperdoll_hair);
        BYTE_WRITE(p, enter_world_packet.empty, sizeof(enter_world_packet.empty));
        BYTE_WRITE_VAL(p, enter_world_packet.p_attack);
        BYTE_WRITE_VAL(p, enter_world_packet.p_attack_speed);
        BYTE_WRITE_VAL(p, enter_world_packet.p_def);
        BYTE_WRITE_VAL(p, enter_world_packet.evasion_rate);
        BYTE_WRITE_VAL(p, enter_world_packet.accuracy);
        BYTE_WRITE_VAL(p, enter_world_packet.critical_hit);
        BYTE_WRITE_VAL(p, enter_world_packet.m_attack);
        BYTE_WRITE_VAL(p, enter_world_packet.m_attack_speed);
        BYTE_WRITE_VAL(p, enter_world_packet.p_attack_speed_copy);
        BYTE_WRITE_VAL(p, enter_world_packet.m_def);
        BYTE_WRITE_VAL(p, enter_world_packet.pvp);
        BYTE_WRITE_VAL(p, enter_world_packet.karma);
        BYTE_WRITE_VAL(p, enter_world_packet.run_speed);
        BYTE_WRITE_VAL(p, enter_world_packet.walk_speed);
        BYTE_WRITE_VAL(p, enter_world_packet.swim_run_speed);
        BYTE_WRITE_VAL(p, enter_world_packet.swim_walk_speed);
        BYTE_WRITE_VAL(p, enter_world_packet.fly_run_speed);
        BYTE_WRITE_VAL(p, enter_world_packet.fly_walk_speed);
        BYTE_WRITE_VAL(p, enter_world_packet.fly_run_speed_copy);
        BYTE_WRITE_VAL(p, enter_world_packet.fly_walk_speed_copy);
        BYTE_WRITE_VAL(p, enter_world_packet.movement_speed_multiplier);
        BYTE_WRITE_VAL(p, enter_world_packet.attack_speed_multiplier);
        BYTE_WRITE_VAL(p, enter_world_packet.collision_radius);
        BYTE_WRITE_VAL(p, enter_world_packet.collision_height);
        BYTE_WRITE_VAL(p, enter_world_packet.hair_style_id);
        BYTE_WRITE_VAL(p, enter_world_packet.hair_color_id);
        BYTE_WRITE_VAL(p, enter_world_packet.face);
        BYTE_WRITE_VAL(p, enter_world_packet.access_level);
        BYTE_WRITE(p, enter_world_packet.title, l2_string_bytes(enter_world_packet.title));
        BYTE_WRITE_VAL(p, enter_world_packet.clan_id);
        BYTE_WRITE_VAL(p, enter_world_packet.clan_crest_id);
        BYTE_WRITE_VAL(p, enter_world_packet.ally_id);
        BYTE_WRITE_VAL(p, enter_world_packet.ally_crest_id);
        BYTE_WRITE_VAL(p, enter_world_packet.clan_leader);
        BYTE_WRITE_VAL(p, enter_world_packet.mount_type);
        BYTE_WRITE_VAL(p, enter_world_packet.private_store_type);
        BYTE_WRITE_VAL(p, enter_world_packet.dwarven_craft);
        BYTE_WRITE_VAL(p, enter_world_packet.pk_kills);
        BYTE_WRITE_VAL(p, enter_world_packet.pvp_kills);
        BYTE_WRITE_VAL(p, enter_world_packet.cubics);
        BYTE_WRITE_VAL(p, enter_world_packet.party_members);
        BYTE_WRITE_VAL(p, enter_world_packet.abnormal_effect);
        BYTE_WRITE_VAL(p, enter_world_packet.char_empty[0]);
        BYTE_WRITE_VAL(p, enter_world_packet.clan_privileges);
        BYTE_WRITE(p, enter_world_packet.int_empty, sizeof(enter_world_packet.int_empty[0]) * 7);
        BYTE_WRITE_VAL(p, enter_world_packet.recommendation_left);
        BYTE_WRITE_VAL(p, enter_world_packet.recommendation_have);
        BYTE_WRITE_VAL(p, enter_world_packet.int_empty[0]);
        BYTE_WRITE_VAL(p, enter_world_packet.inventory_limit);
        BYTE_WRITE_VAL(p, enter_world_packet.class_id_copy);
        BYTE_WRITE_VAL(p, enter_world_packet.int_empty[0]);
        BYTE_WRITE_VAL(p, enter_world_packet.max_cp);
        BYTE_WRITE_VAL(p, enter_world_packet.cp);
        BYTE_WRITE_VAL(p, enter_world_packet.mounted);
        BYTE_WRITE_VAL(p, enter_world_packet.char_empty[0]);
        BYTE_WRITE_VAL(p, enter_world_packet.clan_crest_large_id);
        BYTE_WRITE_VAL(p, enter_world_packet.hero_symbol);
        BYTE_WRITE_VAL(p, enter_world_packet.hero);
        BYTE_WRITE_VAL(p, enter_world_packet.char_empty[0]);
        BYTE_WRITE_VAL(p, enter_world_packet.fish_x);
        BYTE_WRITE_VAL(p, enter_world_packet.fish_y);
        BYTE_WRITE_VAL(p, enter_world_packet.fish_z);
        BYTE_WRITE_VAL(p, enter_world_packet.name_color);

        packet_build(dest, type, buf, (size_t) (p - buf));
}
