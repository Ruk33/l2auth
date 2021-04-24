#ifndef LIB_DB_PLAYER_H
#define LIB_DB_PLAYER_H

#include <headers.h>
#include <character.h>
#include "conn.h"

/**
 * Save playable character under account.
 */
void db_player_add(db_conn_t *db, char *account, character_t *character);

/**
 * Get up to max characters associated to account.
 */
size_t db_player_all_from_account(
        db_conn_t *  db,
        char *       account,
        character_t *dest,
        size_t       max);

/**
 * Get character i (index) from account.
 */
void db_player_by_index(db_conn_t *db, char *account, character_t *dest, int i);

#endif
