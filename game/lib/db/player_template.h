#ifndef LIB_DB_PLAYER_TEMPLATE_H
#define LIB_DB_PLAYER_TEMPLATE_H

#include <headers.h>
#include <player_template.h>
#include <race_id.h>
#include <class_id.h>
#include <db/conn.h>

/**
 * Find player template by race and class id.
 * If found, store it on dest and return 1.
 * If not found, return 0.
 */
int db_player_template_find_by(
        db_conn_t *        db,
        player_template_t *dest,
        race_id_t          race_id,
        class_id_t         class_id);

#endif
