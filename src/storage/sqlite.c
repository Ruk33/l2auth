#include <sqlite3.h>
#include "../include/util.h"
#include "../include/log.h"
#include "../include/gs_types.h"
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

static void sqlite_to_character(struct gs_character *dest, sqlite3_stmt *stmt)
{
        int col = 0;

        col = 1;

        sqlite_cpy_text((byte_t *) (dest->name), stmt, col++);
        dest->race                 = sqlite3_column_int(stmt, col++);
        dest->sex                  = sqlite3_column_int(stmt, col++);
        dest->_class               = sqlite3_column_int(stmt, col++);
        dest->stats._int           = sqlite3_column_int(stmt, col++);
        dest->stats.str            = sqlite3_column_int(stmt, col++);
        dest->stats.con            = sqlite3_column_int(stmt, col++);
        dest->stats.men            = sqlite3_column_int(stmt, col++);
        dest->stats.dex            = sqlite3_column_int(stmt, col++);
        dest->stats.wit            = sqlite3_column_int(stmt, col++);
        dest->hair_style           = sqlite3_column_int(stmt, col++);
        dest->hair_color           = sqlite3_column_int(stmt, col++);
        dest->face                 = sqlite3_column_int(stmt, col++);
        dest->level                = sqlite3_column_int(stmt, col++);
        dest->exp                  = sqlite3_column_int(stmt, col++);
        dest->sp                   = sqlite3_column_int(stmt, col++);
        dest->stats.hp             = sqlite3_column_int(stmt, col++);
        dest->stats.mp             = sqlite3_column_int(stmt, col++);
        dest->stats.cp             = sqlite3_column_int(stmt, col++);
        dest->stats.max_hp         = sqlite3_column_int(stmt, col++);
        dest->stats.max_mp         = sqlite3_column_int(stmt, col++);
        dest->stats.max_cp         = sqlite3_column_int(stmt, col++);
        dest->stats.p_attack       = sqlite3_column_int(stmt, col++);
        dest->stats.m_attack       = sqlite3_column_int(stmt, col++);
        dest->stats.p_def          = sqlite3_column_int(stmt, col++);
        dest->stats.m_def          = sqlite3_column_int(stmt, col++);
        dest->stats.evasion_rate   = sqlite3_column_int(stmt, col++);
        dest->stats.accuracy       = sqlite3_column_int(stmt, col++);
        dest->stats.critical_hit   = sqlite3_column_int(stmt, col++);
        dest->stats.run_speed      = sqlite3_column_int(stmt, col++);
        dest->stats.walk_speed     = sqlite3_column_int(stmt, col++);
        dest->stats.p_attack_speed = sqlite3_column_int(stmt, col++);
        dest->stats.m_attack_speed = sqlite3_column_int(stmt, col++);

        dest->stats.movement_speed_multiplier = sqlite3_column_int(stmt, col++);
        dest->stats.attack_speed_multiplier   = sqlite3_column_int(stmt, col++);

        dest->collision_radius = sqlite3_column_int(stmt, col++);
        dest->collision_height = sqlite3_column_int(stmt, col++);
        dest->name_color       = sqlite3_column_int(stmt, col++);
        dest->max_load         = sqlite3_column_int(stmt, col++);
        dest->position.x       = sqlite3_column_int(stmt, col++);
        dest->position.y       = sqlite3_column_int(stmt, col++);
        dest->position.z       = sqlite3_column_int(stmt, col++);
}

static void conn_open(void)
{
        sqlite_ok_or_log(sqlite3_open("db", &conn));
}

static void conn_close(void)
{
        sqlite3_close(conn);
}

size_t
storage_get_characters(struct gs_character *dest, char *username, size_t max)
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

int storage_create_character(char *username, struct gs_character *src)
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
        bind_int_attr(stmt, src, stats._int);
        bind_int_attr(stmt, src, stats.str);
        bind_int_attr(stmt, src, stats.con);
        bind_int_attr(stmt, src, stats.men);
        bind_int_attr(stmt, src, stats.dex);
        bind_int_attr(stmt, src, stats.wit);
        bind_int_attr(stmt, src, hair_style);
        bind_int_attr(stmt, src, hair_color);
        bind_int_attr(stmt, src, face);
        bind_int_attr(stmt, src, level);
        bind_int_attr(stmt, src, exp);
        bind_int_attr(stmt, src, sp);
        bind_int_attr(stmt, src, stats.hp);
        bind_int_attr(stmt, src, stats.mp);
        bind_int_attr(stmt, src, stats.cp);
        bind_int_attr(stmt, src, stats.max_hp);
        bind_int_attr(stmt, src, stats.max_mp);
        bind_int_attr(stmt, src, stats.max_cp);
        bind_int_attr(stmt, src, stats.p_attack);
        bind_int_attr(stmt, src, stats.m_attack);
        bind_int_attr(stmt, src, stats.p_def);
        bind_int_attr(stmt, src, stats.m_def);
        bind_int_attr(stmt, src, stats.evasion_rate);
        bind_int_attr(stmt, src, stats.accuracy);
        bind_int_attr(stmt, src, stats.critical_hit);
        bind_int_attr(stmt, src, stats.run_speed);
        bind_int_attr(stmt, src, stats.walk_speed);
        bind_int_attr(stmt, src, stats.p_attack_speed);
        bind_int_attr(stmt, src, stats.m_attack_speed);
        bind_int_attr(stmt, src, stats.movement_speed_multiplier);
        bind_int_attr(stmt, src, stats.attack_speed_multiplier);
        bind_int_attr(stmt, src, collision_radius);
        bind_int_attr(stmt, src, collision_height);
        bind_int_attr(stmt, src, name_color);
        bind_int_attr(stmt, src, max_load);
        bind_int_attr(stmt, src, position.x);
        bind_int_attr(stmt, src, position.y);
        bind_int_attr(stmt, src, position.z);

        sqlite_step_and_finalize(stmt);

        result = sqlite_last_op_ok();

        conn_close();

        return result;
}
