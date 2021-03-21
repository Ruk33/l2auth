#ifndef LIB_PLAYER_TEMPLATE_H
#define LIB_PLAYER_TEMPLATE_H

#include "class_id.h"
#include "race_id.h"
#include "stats.h"
#include "position.h"

typedef struct {
        class_id_t class_id;
        race_id_t  race_id;
        stats_t    stats;
        position_t position;
        int        can_craft;
} player_template_t;

#endif
