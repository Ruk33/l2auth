#ifndef CHARACTER_H
#define CHARACTER_H

#include "vec3.h"

struct Character {
        int id;
        char name[35];
        char title[16];
        int level;
        int str;
        int con;
        int dex;
        int _int;
        int men;
        int wit;
        int sex;
        int exp;
        int sp;
        int p_attack;
        int p_defense;
        int m_attack;
        int m_defense;
        int attack_speed;
        int m_attack_speed;
        int walk_speed;
        int run_speed;
        double hp;
        double current_hp;
        double mp;
        double current_mp;
        int heading;
        int x;
        int y;
        int z;
        int accuracy;
        int critical;
        int evasion;
};

#endif
