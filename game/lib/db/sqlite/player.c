#include <headers.h>
#include <sqlite3.h>
#include <character.h>
#include "helper.h"
#include "../player.h"

#define player_characters_columns                              \
        "state, id, name, title, hp, mp, max_hp, max_mp, "     \
        "cp, max_cp, level, clan_id, sex, race_id, class_id, " \
        "x, y, z, sp, exp, karma, _int, str, con, men, dex, "  \
        "wit, p_attack, m_attack, p_def, m_def, evasion_rate, critical_hit"

static void sqlite_to_character(sqlite3_stmt *stmt, character_t *dest)
{
        size_t name_s  = 0;
        size_t title_s = 0;

        int c = 0;

        assert(stmt);
        assert(dest);

        name_s  = sizeof(dest->name);
        title_s = sizeof(dest->title);

        dest->state = sqlite3_column_int(stmt, c++);
        dest->id    = sqlite3_column_int(stmt, c++);
        strncpy(dest->name, (char *) sqlite3_column_text(stmt, c++), name_s);
        strncpy(dest->title, (char *) sqlite3_column_text(stmt, c++), title_s);
        dest->hp           = sqlite3_column_double(stmt, c++);
        dest->mp           = sqlite3_column_double(stmt, c++);
        dest->max_hp       = sqlite3_column_double(stmt, c++);
        dest->max_mp       = sqlite3_column_double(stmt, c++);
        dest->cp           = sqlite3_column_int(stmt, c++);
        dest->max_cp       = sqlite3_column_int(stmt, c++);
        dest->level        = sqlite3_column_int(stmt, c++);
        dest->clan_id      = sqlite3_column_int(stmt, c++);
        dest->sex          = sqlite3_column_int(stmt, c++);
        dest->race_id      = sqlite3_column_int(stmt, c++);
        dest->class_id     = sqlite3_column_int(stmt, c++);
        dest->x            = sqlite3_column_int(stmt, c++);
        dest->y            = sqlite3_column_int(stmt, c++);
        dest->z            = sqlite3_column_int(stmt, c++);
        dest->sp           = sqlite3_column_int(stmt, c++);
        dest->exp          = sqlite3_column_int(stmt, c++);
        dest->karma        = sqlite3_column_int(stmt, c++);
        dest->_int         = sqlite3_column_int(stmt, c++);
        dest->str          = sqlite3_column_int(stmt, c++);
        dest->con          = sqlite3_column_int(stmt, c++);
        dest->men          = sqlite3_column_int(stmt, c++);
        dest->dex          = sqlite3_column_int(stmt, c++);
        dest->wit          = sqlite3_column_int(stmt, c++);
        dest->p_attack     = sqlite3_column_int(stmt, c++);
        dest->m_attack     = sqlite3_column_int(stmt, c++);
        dest->p_def        = sqlite3_column_int(stmt, c++);
        dest->m_def        = sqlite3_column_int(stmt, c++);
        dest->evasion_rate = sqlite3_column_int(stmt, c++);
        dest->critical_hit = sqlite3_column_int(stmt, c++);
}

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
        char query[] =
                "select " player_characters_columns
                " from player_characters where account = :account limit :max;";

        sqlite3_stmt *stmt = NULL;

        size_t found = 0;

        assert(db);
        assert(account);
        assert(dest);

        sqlite3_prepare_v2(db, query, sizeof(query) - 1, &stmt, NULL);

        helper_bind_text(stmt, ":account", account);
        helper_bind_int(stmt, ":max", (int) max);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite_to_character(stmt, &dest[found]);
                found += 1;
        }

        sqlite3_finalize(stmt);

        return found;
}

void db_player_by_index(db_conn_t *db, char *account, character_t *dest, int i)
{
        assert(db);
        assert(account);
        assert(dest);
        printf("Get characters from %s using index %d. TODO!", account, i);
}
