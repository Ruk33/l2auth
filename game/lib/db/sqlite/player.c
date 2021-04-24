#include <headers.h>
#include <character.h>
#include "../player.h"

void db_player_add(db_conn_t *db, char *account, character_t *character)
{
        assert(db);
        assert(account);
        assert(character);
        printf("Save playable character, TODO!");
}

size_t db_player_all_from_account(
        db_conn_t *  db,
        char *       account,
        character_t *dest,
        size_t       max)
{
        assert(db);
        assert(account);
        assert(dest);
        if (max) {
        }
        printf("Get all characters from %s. TODO!", account);
        return 0;
}

void db_player_by_index(db_conn_t *db, char *account, character_t *dest, int i)
{
        assert(db);
        assert(account);
        assert(dest);
        printf("Get characters from %s using index %d. TODO!", account, i);
}
