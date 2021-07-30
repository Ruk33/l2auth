#include "include/util.h"
#include "include/gs_types.h"

static struct gs_character_template default_templates[] = {
        // Human fighter
        { .race   = 0,
          ._class = 0,
          .stats  = { .str  = 40,
                     .dex  = 30,
                     .con  = 43,
                     ._int = 21,
                     .wit  = 11,
                     .men  = 25 } },

        // Human fighter copy
        // Not sure why the clients requires it...
        { .race   = 0,
          ._class = 0,
          .stats  = { .str  = 40,
                     .dex  = 30,
                     .con  = 43,
                     ._int = 21,
                     .wit  = 11,
                     .men  = 25 } },

        // Human mage
        { .race   = 0,
          ._class = 10,
          .stats  = { .str  = 22,
                     .dex  = 21,
                     .con  = 27,
                     ._int = 41,
                     .wit  = 20,
                     .men  = 39 } },

        // Elf figher
        { .race   = 1,
          ._class = 18,
          .stats  = { .str  = 36,
                     .dex  = 35,
                     .con  = 36,
                     ._int = 23,
                     .wit  = 14,
                     .men  = 26 } },

        // Elf mage
        { .race   = 1,
          ._class = 25,
          .stats  = { .str  = 21,
                     .dex  = 24,
                     .con  = 25,
                     ._int = 37,
                     .wit  = 23,
                     .men  = 40 } },

        // Dark elf fighter
        { .race   = 2,
          ._class = 31,
          .stats  = { .str  = 41,
                     .dex  = 34,
                     .con  = 32,
                     ._int = 25,
                     .wit  = 12,
                     .men  = 26 } },

        // Dark elf mage
        { .race   = 2,
          ._class = 38,
          .stats  = { .str  = 23,
                     .dex  = 23,
                     .con  = 24,
                     ._int = 44,
                     .wit  = 19,
                     .men  = 37 } },

        // Orc fighter
        { .race   = 3,
          ._class = 44,
          .stats  = { .str  = 40,
                     .dex  = 26,
                     .con  = 47,
                     ._int = 18,
                     .wit  = 12,
                     .men  = 27 } },

        // Orc mage
        { .race   = 3,
          ._class = 49,
          .stats  = { .str  = 27,
                     .dex  = 24,
                     .con  = 31,
                     ._int = 31,
                     .wit  = 15,
                     .men  = 42 } },

        // Dwarf
        { .race   = 4,
          ._class = 53,
          .stats  = { .str  = 39,
                     .dex  = 29,
                     .con  = 45,
                     ._int = 20,
                     .wit  = 10,
                     .men  = 27 } }
};

struct gs_character_template *gs_character_template_default(void)
{
        return default_templates;
}

size_t gs_character_template_count(void)
{
        return sizeof(default_templates) / sizeof(default_templates[0]);
}
