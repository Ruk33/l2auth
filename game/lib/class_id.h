#ifndef LIB_CLASS_ID_H
#define LIB_CLASS_ID_H

/**
 * Thanks to
 * https://bitbucket.org/l2jserver/l2j-server-datapack/src/C4/L2J_DataPack/sql/char_templates.sql
 */
typedef enum {
        CLASS_ID_HUMAN_FIGHTER    = 0,
        CLASS_ID_HUMAN_MAGE       = 10,
        CLASS_ID_ELF_FIGHTER      = 18,
        CLASS_ID_ELF_MAGE         = 25,
        CLASS_ID_DARK_ELF_FIGHTER = 31,
        CLASS_ID_DARK_ELF_MAGE    = 38,
        CLASS_ID_ORC_FIGHTER      = 44,
        CLASS_ID_ORC_MAGE         = 49,
        CLASS_ID_DWARF_FIGHER     = 53,
} class_id_t;

#endif
