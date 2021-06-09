#include <sqlite3.h>
#include "../include/util.h"
#include "../include/log.h"
#include "../include/gs_character.h"
#include "../include/storage.h"

// Log if operation didn't executed well.
#define sqlite_ok_or_log(op)                              \
        if (op != SQLITE_OK) {                            \
                log("%s", (char *) sqlite3_errmsg(conn)); \
        }

/**
 * Simple query initialization.
 *
 * Example:
 * sqlite3_stmt *stmt = 0;
 * sqlite_query(stmt, "select * from table;");
 */
#define sqlite_query(stmt, query) \
        sqlite_ok_or_log(         \
                sqlite3_prepare_v2(conn, query, sizeof(query) - 1, &stmt, 0))

/**
 * Useful for statements where the result isn't needed.
 * If step or finalize don't succeed, an error message will be logged.
 */
#define sqlite_step_and_finalize(stmt)                    \
        do {                                              \
                sqlite_ok_or_log(sqlite3_step(stmt));     \
                sqlite_ok_or_log(sqlite3_finalize(stmt)); \
        } while (0)

// Check if last sqlite operation went well.
#define sqlite_last_op_ok() sqlite3_errcode(conn) == SQLITE_OK

/**
 * Bind bytes to key from query.
 * Intended to be used with fixed sized values.
 *
 * Example:
 * char[] foo = "bar";
 * db_sqlite_util_bind_bytes(stmt, ":foo", foo);
 */
#define sqlite_bind_bytes(stmt, key, value)              \
        sqlite3_bind_blob(                               \
                stmt,                                    \
                sqlite3_bind_parameter_index(stmt, key), \
                value,                                   \
                sizeof(value),                           \
                NULL)

/**
 * Bind n bytes from text.
 */
#define sqlite_bind_n_text(stmt, key, value, n) \
        sqlite_ok_or_log(sqlite3_bind_text(     \
                stmt, sqlite3_bind_parameter_index(stmt, key), value, n, NULL))

/**
 * Bind safe null string terminated to key from query.
 *
 * Example:
 * char[] foo = "bar";
 * sqlite_bind_text(stmt, ":foo", foo);
 */
#define sqlite_bind_text(stmt, key, value) \
        sqlite_bind_n_text(stmt, key, value, -1)

/**
 * Bind int to query.
 */
#define sqlite_bind_int(stmt, key, value)  \
        sqlite_ok_or_log(sqlite3_bind_int( \
                stmt, sqlite3_bind_parameter_index(stmt, key), value))

#define bind_text_attr(stmt, src, attr) \
        sqlite_bind_text(stmt, ":" #attr, src->attr)

#define bind_int_attr(stmt, src, attr) \
        sqlite_bind_int(stmt, ":" #attr, src->attr)

/**
 * Safely copy text value.
 * Keep in mind sqlite3_column_text could return NULL,
 * that's why the ternary operator is being used.
 */
#define sqlite_cpy_text(dest, stmt, col)                   \
        bytes_cpy_str(                                     \
                dest,                                      \
                (byte_t *) sqlite3_column_text(stmt, col), \
                sizeof(dest) - 1)

#define CHARACTERS_COLUMNS                                                                     \
        "username, name, race, sex, _class, _int, str, con, men, dex, "                        \
        "wit, hair_style, hair_color, face, level, exp, sp, hp, mp, "                          \
        "cp, max_hp, max_mp, max_cp, p_attack, m_attack, p_def, "                              \
        "m_def, evasion_rate, accuracy, critical_hit, run_speed, walk_speed, "                 \
        "p_attack_speed, m_attack_speed, movement_speed_multiplier, attack_speed_multiplier, " \
        "collision_radius, collision_height, name_color, max_load, x, y, z"

#define CHARACTERS_BY_USERNAME_QUERY \
        "select " CHARACTERS_COLUMNS \
        " from characters where username = :username limit :limit;"

#define CHARACTERS_CREATE_QUERY                                                                   \
        "insert into characters (" CHARACTERS_COLUMNS                                             \
        ") values (:username, :name, :race, :sex, :_class, :_int, :str, "                         \
        ":con, :men, :dex, :wit, :hair_style, :hair_color, :face, "                               \
        ":level, :exp, :sp, :hp, :mp, :cp, :max_hp, :max_mp, :max_cp, "                           \
        ":p_attack, :m_attack, :p_def, :m_def, :evasion_rate, :accuracy, :critical_hit, "         \
        ":run_speed, :walk_speed, :p_attack_speed, :m_attack_speed, :movement_speed_multiplier, " \
        ":attack_speed_multiplier, :collision_radius, :collision_height, "                        \
        ":name_color, :max_load, :x, :y, :z);"

static sqlite3 *conn = 0;

static void sqlite_to_character(gs_character_t *dest, sqlite3_stmt *stmt)
{
        int col = 0;

        col = 1;

        sqlite_cpy_text((byte_t *) (dest->name), stmt, col++);
        dest->race           = sqlite3_column_int(stmt, col++);
        dest->sex            = sqlite3_column_int(stmt, col++);
        dest->_class         = sqlite3_column_int(stmt, col++);
        dest->_int           = sqlite3_column_int(stmt, col++);
        dest->str            = sqlite3_column_int(stmt, col++);
        dest->con            = sqlite3_column_int(stmt, col++);
        dest->men            = sqlite3_column_int(stmt, col++);
        dest->dex            = sqlite3_column_int(stmt, col++);
        dest->wit            = sqlite3_column_int(stmt, col++);
        dest->hair_style     = sqlite3_column_int(stmt, col++);
        dest->hair_color     = sqlite3_column_int(stmt, col++);
        dest->face           = sqlite3_column_int(stmt, col++);
        dest->level          = sqlite3_column_int(stmt, col++);
        dest->exp            = sqlite3_column_int(stmt, col++);
        dest->sp             = sqlite3_column_int(stmt, col++);
        dest->hp             = sqlite3_column_int(stmt, col++);
        dest->mp             = sqlite3_column_int(stmt, col++);
        dest->cp             = sqlite3_column_int(stmt, col++);
        dest->max_hp         = sqlite3_column_int(stmt, col++);
        dest->max_mp         = sqlite3_column_int(stmt, col++);
        dest->max_cp         = sqlite3_column_int(stmt, col++);
        dest->p_attack       = sqlite3_column_int(stmt, col++);
        dest->m_attack       = sqlite3_column_int(stmt, col++);
        dest->p_def          = sqlite3_column_int(stmt, col++);
        dest->m_def          = sqlite3_column_int(stmt, col++);
        dest->evasion_rate   = sqlite3_column_int(stmt, col++);
        dest->accuracy       = sqlite3_column_int(stmt, col++);
        dest->critical_hit   = sqlite3_column_int(stmt, col++);
        dest->run_speed      = sqlite3_column_int(stmt, col++);
        dest->walk_speed     = sqlite3_column_int(stmt, col++);
        dest->p_attack_speed = sqlite3_column_int(stmt, col++);
        dest->m_attack_speed = sqlite3_column_int(stmt, col++);

        dest->movement_speed_multiplier = sqlite3_column_int(stmt, col++);
        dest->attack_speed_multiplier   = sqlite3_column_int(stmt, col++);

        dest->collision_radius = sqlite3_column_int(stmt, col++);
        dest->collision_height = sqlite3_column_int(stmt, col++);
        dest->name_color       = sqlite3_column_int(stmt, col++);
        dest->max_load         = sqlite3_column_int(stmt, col++);
        dest->x                = sqlite3_column_int(stmt, col++);
        dest->y                = sqlite3_column_int(stmt, col++);
        dest->z                = sqlite3_column_int(stmt, col++);
}

static void conn_open(void)
{
        sqlite_ok_or_log(sqlite3_open("db", &conn));
}

static void conn_close(void)
{
        sqlite3_close(conn);
}

size_t storage_get_characters(gs_character_t *dest, char *username, size_t max)
{
        sqlite3_stmt *stmt = 0;

        size_t found = 0;

        conn_open();

        sqlite_query(stmt, CHARACTERS_BY_USERNAME_QUERY);

        sqlite_bind_text(stmt, ":username", username);
        sqlite_bind_int(stmt, ":limit", (int) max);

        while (found < max && sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite_to_character(&dest[found], stmt);
                found += 1;
        }

        conn_close();

        return found;
}

int storage_create_character(char *username, gs_character_t *src)
{
        sqlite3_stmt *stmt = 0;

        int result = 0;

        conn_open();

        sqlite_query(stmt, CHARACTERS_CREATE_QUERY);

        sqlite_bind_text(stmt, ":username", username);
        bind_text_attr(stmt, src, name);
        bind_int_attr(stmt, src, race);
        bind_int_attr(stmt, src, sex);
        bind_int_attr(stmt, src, _class);
        bind_int_attr(stmt, src, _int);
        bind_int_attr(stmt, src, str);
        bind_int_attr(stmt, src, con);
        bind_int_attr(stmt, src, men);
        bind_int_attr(stmt, src, dex);
        bind_int_attr(stmt, src, wit);
        bind_int_attr(stmt, src, hair_style);
        bind_int_attr(stmt, src, hair_color);
        bind_int_attr(stmt, src, face);
        bind_int_attr(stmt, src, level);
        bind_int_attr(stmt, src, exp);
        bind_int_attr(stmt, src, sp);
        bind_int_attr(stmt, src, hp);
        bind_int_attr(stmt, src, mp);
        bind_int_attr(stmt, src, cp);
        bind_int_attr(stmt, src, max_hp);
        bind_int_attr(stmt, src, max_mp);
        bind_int_attr(stmt, src, max_cp);
        bind_int_attr(stmt, src, p_attack);
        bind_int_attr(stmt, src, m_attack);
        bind_int_attr(stmt, src, p_def);
        bind_int_attr(stmt, src, m_def);
        bind_int_attr(stmt, src, evasion_rate);
        bind_int_attr(stmt, src, accuracy);
        bind_int_attr(stmt, src, critical_hit);
        bind_int_attr(stmt, src, run_speed);
        bind_int_attr(stmt, src, walk_speed);
        bind_int_attr(stmt, src, p_attack_speed);
        bind_int_attr(stmt, src, m_attack_speed);
        bind_int_attr(stmt, src, movement_speed_multiplier);
        bind_int_attr(stmt, src, attack_speed_multiplier);
        bind_int_attr(stmt, src, collision_radius);
        bind_int_attr(stmt, src, collision_height);
        bind_int_attr(stmt, src, name_color);
        bind_int_attr(stmt, src, max_load);
        bind_int_attr(stmt, src, x);
        bind_int_attr(stmt, src, y);
        bind_int_attr(stmt, src, z);

        sqlite_step_and_finalize(stmt);

        result = sqlite_last_op_ok();

        conn_close();

        return result;
}
