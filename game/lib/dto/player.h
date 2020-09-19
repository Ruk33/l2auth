#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

struct Player {
        struct Character character;
        char account_name[45];
        int max_cp;
        int face;
        int hair_style_id;
        int hair_color_id;
        int movement_multiplier;
        int attack_speed_multiplier;
        int karma;
        int pvp_kills;
        int pk_kills;
        int clan_id;
        int max_load;
        int race_id;
        int class_id;
        int base_class_id;
        int delete_time;
        int can_craft;
        int ally_id;
        int recommendations;
        int recommendations_left;
        int access_level;
        int online;
        int online_time;
        int slot;
        int newbie;
        int last_access;
        int clan_privileges;
        int wants_peace; // No, it will never be an option
        int delete_clan;
        int in_7signs_dungeon;
        int in_jail;
        int jail_timer;
        int active;
};

#endif
