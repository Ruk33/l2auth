#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_string.h>
#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <core/byte_builder.h>
#include <game/dto/char.h>
#include <game/packet/enter_world.h>

l2_packet* game_packet_enter_world(struct L2Client* client)
{
        assert(client);

        l2_packet_type type = 0x04;
        byte_builder* buffer = l2_client_byte_builder(client, 1000);

        int heading = 0;
        int exp = 10;
        unsigned int sp = 10;
        unsigned int current_load = 0;
        unsigned int max_load = 10;
        unsigned int unknown = 0x28;
        unsigned int paperdoll_under = 0x00;
        unsigned int paperdoll_rear = 0x00;
        unsigned int paperdoll_lear = 0x00;
        unsigned int paperdoll_neck = 0x00;
        unsigned int paperdoll_rfinger = 0x00;
        unsigned int paperdoll_lfinger = 0x00;
        unsigned int paperdoll_head = 0x00;
        unsigned int paperdoll_rhand = 0x00;
        unsigned int paperdoll_lhand = 0x00;
        unsigned int paperdoll_gloves = 0x00;
        unsigned int paperdoll_chest = 0x00;
        unsigned int paperdoll_legs = 0x00;
        unsigned int paperdoll_feet = 0x00;
        unsigned int paperdoll_back = 0x00;
        unsigned int paperdoll_lrhand = 0;
        unsigned int paperdoll_hair = 0x00;
        unsigned int p_atk = 20;
        unsigned int p_atk_speed = 20;
        unsigned int p_def = 20;
        unsigned int evasion_rate = 1;
        unsigned int accuracy = 10;
        unsigned int critical_hit = 1;
        unsigned int m_atk = 1;
        unsigned int m_atk_speed = 1;
        unsigned int m_def = 20;
        unsigned int pvp = 0;
        unsigned int karma = 0;
        unsigned int run_speed = 100;
        unsigned int walk_speed = 100;
        unsigned int swim_run_speed = 10;
        unsigned int swim_walk_speed = 10;
        unsigned int fly_run_speed = 10;
        unsigned int fly_walk_speed = 10;
        double movement_speed_multiplier = 1;
        double atk_speed_multiplier = 1;
        double collision_radius = 9;
        double collision_height = 23;
        unsigned int access_level = 0;
        unsigned char title[] = { 'h', 0, 'i', 0, 0, 0 };
        unsigned int clan_id = 0;
        unsigned int clan_crest_id = 0;
        unsigned int ally_id = 0;
        unsigned int ally_crest_id = 0;
        unsigned int clan_leader = 0;
        unsigned char mount_type[] = { 0 };
        unsigned char private_store_type[] = { 0 };
        unsigned char dwarven_craft[] = { 0 };
        unsigned int pk_kills = 0;
        unsigned int pvp_kills = 10;
        unsigned short cubics = 0;
        unsigned char party_member[] = { 0 };
        unsigned int abnormal_effect = 0;
        unsigned char empty[] = { 0 };
        unsigned int clan_privileges = 0;
        unsigned int empty_int = 0;
        unsigned short recomendation_left = 0;
        unsigned short recomendation_have = 0;
        unsigned short inventory_limit = 10;
        unsigned int max_cp = 10;
        unsigned int cp = 10;
        unsigned char mounted[] = { 0 };
        unsigned int clan_crest_large_id = 0;
        unsigned char hero_symbol[] = { 0 };
        unsigned char hero[] = { 0 };
        unsigned int fish_x = 0;
        unsigned int fish_y = 0;
        unsigned int fish_z = 0;
        unsigned int name_color = 4294967295;
        struct GameDtoChar* character = l2_client_get_char(client);
        l2_string name[28];
        int int_hp = (int) character->current_hp;
        int int_mp = (int) character->current_mp;
        int int_max_hp = (int) character->max_hp;
        int int_max_mp = (int) character->max_mp;

        log_info("Enter world, character information", character->name);
        log_info("Name %s", character->name);
        log_info("Char id %d", character->char_id);
        log_info("Race id %d", character->race_id);
        log_info("Class id %d", character->class_id);
        log_info("Sex %d", character->sex);
        log_info("Int %d", character->_int);
        log_info("Str %d", character->str);
        log_info("Con %d", character->con);
        log_info("Men %d", character->men);
        log_info("Dex %d", character->dex);
        log_info("Wit %d", character->wit);
        log_info("X %d", character->current_location.x);
        log_info("Y %d", character->current_location.y);
        log_info("Z %d", character->current_location.z);
        log_info("Current hp %f", character->current_hp);
        log_info("Current mp %f", character->current_mp);
        log_info("Max hp %f", character->max_hp);
        log_info("Max mp %f", character->max_mp);
        log_info("Level %d", character->level);
        log_info("Hair style %d", character->hair_style);
        log_info("Hair color %d", character->hair_color);
        log_info("Face ID %d", character->face);

        memset(name, 0, 28);
        l2_string_from_char(name, character->name, strlen(character->name));

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
                (unsigned char *) &heading,
                sizeof(heading)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->char_id,
                sizeof(character->char_id)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) name,
                l2_string_calculate_space_from_char(strlen(character->name) + 1)
        );

        byte_builder_append(
                buffer,
                (unsigned char *) &character->race_id,
                sizeof(character->race_id)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &character->sex,
                sizeof(character->sex)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &character->class_id,
                sizeof(character->class_id)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &character->level,
                sizeof(character->level)
        );
        byte_builder_append(buffer, (unsigned char *) &exp, sizeof(exp));
        byte_builder_append(
                buffer,
                (unsigned char *) &character->str,
                sizeof(character->str)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &character->dex,
                sizeof(character->dex)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &character->con,
                sizeof(character->con)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &character->_int,
                sizeof(character->_int)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &character->wit,
                sizeof(character->wit)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &character->men,
                sizeof(character->men)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &int_max_hp,
                sizeof(int_max_hp)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &int_hp,
                sizeof(int_hp)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &int_max_mp,
                sizeof(int_max_mp)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &int_mp,
                sizeof(int_mp)
        );
        byte_builder_append(buffer, (unsigned char *) &sp, sizeof(sp));
        byte_builder_append(buffer, (unsigned char *) &current_load, sizeof(current_load));
        byte_builder_append(buffer, (unsigned char *) &max_load, sizeof(max_load));
        byte_builder_append(buffer, (unsigned char *) &unknown, sizeof(unknown));

        byte_builder_append(buffer, (unsigned char *) &paperdoll_under, sizeof(paperdoll_under));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_rear, sizeof(paperdoll_rear));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lear, sizeof(paperdoll_lear));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_neck, sizeof(paperdoll_neck));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_rfinger, sizeof(paperdoll_rfinger));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lfinger, sizeof(paperdoll_lfinger));
        
        byte_builder_append(buffer, (unsigned char *) &paperdoll_head, sizeof(paperdoll_head));

        byte_builder_append(buffer, (unsigned char *) &paperdoll_rhand, sizeof(paperdoll_rhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lhand, sizeof(paperdoll_lhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_gloves, sizeof(paperdoll_gloves));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_chest, sizeof(paperdoll_chest));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_legs, sizeof(paperdoll_legs));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_feet, sizeof(paperdoll_feet));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_back, sizeof(paperdoll_back));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lrhand, sizeof(paperdoll_lrhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_hair, sizeof(paperdoll_hair));

        byte_builder_append(buffer, (unsigned char *) &paperdoll_under, sizeof(paperdoll_under));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_rear, sizeof(paperdoll_rear));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lear, sizeof(paperdoll_lear));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_neck, sizeof(paperdoll_neck));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_rfinger, sizeof(paperdoll_rfinger));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lfinger, sizeof(paperdoll_lfinger));

        byte_builder_append(buffer, (unsigned char *) &paperdoll_head, sizeof(paperdoll_head));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_rhand, sizeof(paperdoll_rhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lhand, sizeof(paperdoll_lhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_gloves, sizeof(paperdoll_gloves));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_chest, sizeof(paperdoll_chest));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_legs, sizeof(paperdoll_legs));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_feet, sizeof(paperdoll_feet));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_back, sizeof(paperdoll_back));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lrhand, sizeof(paperdoll_lrhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_hair, sizeof(paperdoll_hair));

        byte_builder_append(buffer, (unsigned char *) &p_atk, sizeof(p_atk));
        byte_builder_append(buffer, (unsigned char *) &p_atk_speed, sizeof(p_atk_speed));
        byte_builder_append(buffer, (unsigned char *) &p_def, sizeof(p_def));
        byte_builder_append(buffer, (unsigned char *) &evasion_rate, sizeof(evasion_rate));
        byte_builder_append(buffer, (unsigned char *) &accuracy, sizeof(accuracy));
        byte_builder_append(buffer, (unsigned char *) &critical_hit, sizeof(critical_hit));
        byte_builder_append(buffer, (unsigned char *) &m_atk, sizeof(m_atk));
        byte_builder_append(buffer, (unsigned char *) &m_atk_speed, sizeof(m_atk_speed));
        byte_builder_append(buffer, (unsigned char *) &p_atk_speed, sizeof(p_atk_speed));
        byte_builder_append(buffer, (unsigned char *) &m_def, sizeof(m_def));

        byte_builder_append(buffer, (unsigned char *) &pvp, sizeof(pvp));
        byte_builder_append(buffer, (unsigned char *) &karma, sizeof(karma));

        byte_builder_append(buffer, (unsigned char *) &run_speed, sizeof(run_speed));
        byte_builder_append(buffer, (unsigned char *) &walk_speed, sizeof(walk_speed));
        byte_builder_append(buffer, (unsigned char *) &swim_run_speed, sizeof(swim_run_speed));
        byte_builder_append(buffer, (unsigned char *) &swim_walk_speed, sizeof(swim_walk_speed));
        byte_builder_append(buffer, (unsigned char *) &fly_run_speed, sizeof(fly_run_speed));
        byte_builder_append(buffer, (unsigned char *) &fly_walk_speed, sizeof(fly_walk_speed));
        byte_builder_append(buffer, (unsigned char *) &fly_run_speed, sizeof(fly_run_speed));
        byte_builder_append(buffer, (unsigned char *) &fly_walk_speed, sizeof(fly_walk_speed));
        byte_builder_append(buffer, (unsigned char *) &movement_speed_multiplier, sizeof(movement_speed_multiplier));
        byte_builder_append(buffer, (unsigned char *) &atk_speed_multiplier, sizeof(atk_speed_multiplier));

        byte_builder_append(buffer, (unsigned char *) &collision_radius, sizeof(collision_radius));
        byte_builder_append(buffer, (unsigned char *) &collision_height, sizeof(collision_height));

        byte_builder_append(
                buffer,
                (unsigned char *) &character->hair_style,
                sizeof(character->hair_style)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &character->hair_color,
                sizeof(character->hair_color)
        );
        byte_builder_append(
                buffer,
                (unsigned char *) &character->face,
                sizeof(character->face)
        );
        byte_builder_append(buffer, (unsigned char *) &access_level, sizeof(access_level));

        byte_builder_append(buffer, title, sizeof(title));

        byte_builder_append(buffer, (unsigned char *) &clan_id, sizeof(clan_id));
        byte_builder_append(buffer, (unsigned char *) &clan_crest_id, sizeof(clan_crest_id));
        byte_builder_append(buffer, (unsigned char *) &ally_id, sizeof(ally_id));
        byte_builder_append(buffer, (unsigned char *) &ally_crest_id, sizeof(ally_crest_id));
        byte_builder_append(buffer, (unsigned char *) &clan_leader, sizeof(clan_leader));
        byte_builder_append(buffer, (unsigned char *) &mount_type, sizeof(mount_type));
        byte_builder_append(buffer, (unsigned char *) &private_store_type, sizeof(private_store_type));
        byte_builder_append(buffer, (unsigned char *) &dwarven_craft, sizeof(dwarven_craft));
        byte_builder_append(buffer, (unsigned char *) &pk_kills, sizeof(pk_kills));
        byte_builder_append(buffer, (unsigned char *) &pvp_kills, sizeof(pvp_kills));

        byte_builder_append(buffer, (unsigned char *) &cubics, sizeof(cubics));

        byte_builder_append(buffer, (unsigned char *) &party_member, sizeof(party_member));
        byte_builder_append(buffer, (unsigned char *) &abnormal_effect, sizeof(abnormal_effect));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &clan_privileges, sizeof(clan_privileges));

        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));

        byte_builder_append(buffer, (unsigned char *) &recomendation_left, sizeof(recomendation_left));
        byte_builder_append(buffer, (unsigned char *) &recomendation_have, sizeof(recomendation_have));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &inventory_limit, sizeof(inventory_limit));
        
        byte_builder_append(
                buffer,
                (unsigned char *) &character->class_id,
                sizeof(character->class_id)
        );

        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &max_cp, sizeof(max_cp));
        byte_builder_append(buffer, (unsigned char *) &cp, sizeof(cp));

        byte_builder_append(buffer, (unsigned char *) &mounted, sizeof(mounted));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));

        byte_builder_append(buffer, (unsigned char *) &clan_crest_large_id, sizeof(clan_crest_large_id));
        byte_builder_append(buffer, (unsigned char *) &hero_symbol, sizeof(hero_symbol));
        byte_builder_append(buffer, (unsigned char *) &hero, sizeof(hero));

        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &fish_x, sizeof(fish_x));
        byte_builder_append(buffer, (unsigned char *) &fish_y, sizeof(fish_y));
        byte_builder_append(buffer, (unsigned char *) &fish_z, sizeof(fish_z));
        byte_builder_append(buffer, (unsigned char *) &name_color, sizeof(name_color));

        return l2_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
