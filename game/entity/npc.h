#ifndef L2AUTH_GAME_ENTITY_NPC_H
#define L2AUTH_GAME_ENTITY_NPC_H

#include "character.h"
#include "player.h"

struct Npc {
        struct Character character;
        int template_id;
        int server_side_name;
        char _class[200];
        int collision_radiuos;
        int collision_height;
        char type[20];
        int attack_range;
        int aggro;
        int r_hand_id;
        int l_hand_id;
        int armor_id;
        int faction_id;
        int faction_range;
        int is_undead;
        int absorb_level;
};

// void npc_attack_npc(struct Npc *npc, struct Npc *target);
// void npc_attack_player(struct Npc *npc, struct Player *target);


#endif
