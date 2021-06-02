#ifndef INCLUDE_CHARACTER_TEMPLATE_H
#define INCLUDE_CHARACTER_TEMPLATE_H

#include "util.h"

typedef struct {
        int race;
        int _class;
        int str;
        int dex;
        int con;
        int _int;
        int wit;
        int men;
} character_template_t;

character_template_t *character_template_default(void);

size_t character_template_count(void);

#endif
