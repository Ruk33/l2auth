#include <assert.h>
#include <sqlite3.h>
#include "../include/util.h"
#include "../include/gs_types.h"
#include "../include/ls_types.h"
#include "../include/storage.h"

// Log if operation didn't executed well.
#define sqlite_ok_or_log(op)                                     \
        if ((op) != SQLITE_OK) {                                 \
                log_normal("%s", (char *) sqlite3_errmsg(conn)); \
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
                0)

/**
 * Bind n bytes from text.
 */
#define sqlite_bind_n_text(stmt, key, value, n) \
        sqlite_ok_or_log(sqlite3_bind_text(     \
                stmt, sqlite3_bind_parameter_index(stmt, key), value, n, 0))

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
#define sqlite_cpy_text(dest, stmt, col)                 \
        util_cpy_str(                                    \
                (char *) dest,                           \
                (char *) sqlite3_column_text(stmt, col), \
                sizeof(dest),                            \
                sizeof(dest))

#define ACCOUNT_FROM_USERNAME_QUERY \
        "select username, encrypted_password from accounts where username = :username limit 1;"

#define ACCOUNT_CREATE_QUERY \
        "insert into accounts(username, encrypted_password) values (:username, :encrypted_password);"

#define SERVERS_QUERY \
        "select id, ip, port, age_limit, pvp, players, max_players, status, extra, brackets from servers limit :limit;"

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

#define CHARACTERS_BY_USERNAME_AND_INDEX_QUERY \
        "select " CHARACTERS_COLUMNS           \
        " from characters where username = :username limit 1 offset :index;"

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

        assert(dest);
        assert(stmt);

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

int storage_get_account(struct ls_account *dest, char *username)
{
        sqlite3_stmt *stmt = 0;

        int result = 0;

        assert(dest);
        assert(username);

        conn_open();
        sqlite_query(stmt, ACCOUNT_FROM_USERNAME_QUERY);

        sqlite_bind_text(stmt, ":username", username);

        result = sqlite3_step(stmt) == SQLITE_ROW;

        if (result) {
                sqlite_cpy_text(dest->username, stmt, 0);
                util_cpy_bytes(
                        dest->encrypted_password,
                        (void *) sqlite3_column_blob(stmt, 1),
                        sizeof(dest->encrypted_password),
                        sizeof(dest->encrypted_password),
                        sizeof(dest->encrypted_password));
        }

        sqlite3_finalize(stmt);
        conn_close();

        return result;
}

int storage_create_account(struct ls_account *src)
{
        sqlite3_stmt *stmt = 0;

        int result = 0;

        assert(src);

        conn_open();
        sqlite_query(stmt, ACCOUNT_CREATE_QUERY);

        sqlite_bind_text(stmt, ":username", src->username);
        sqlite_bind_bytes(stmt, ":encrypted_password", src->encrypted_password);

        result = sqlite3_step(stmt) == SQLITE_DONE;

        sqlite3_finalize(stmt);
        conn_close();

        return result;
}

size_t storage_get_servers(struct ls_server *dest, size_t max)
{
        sqlite3_stmt *stmt = 0;

        size_t found = 0;

        assert(dest);

        conn_open();
        sqlite_query(stmt, SERVERS_QUERY);

        sqlite_bind_int(stmt, ":limit", (int) max);

        while (found < max && sqlite3_step(stmt) == SQLITE_ROW) {
                dest[found].id = sqlite3_column_int(stmt, 0);
                sqlite_cpy_text(dest[found].text_ip, stmt, 1);
                dest[found].port        = sqlite3_column_int(stmt, 2);
                dest[found].age_limit   = sqlite3_column_int(stmt, 3);
                dest[found].pvp         = sqlite3_column_int(stmt, 4);
                dest[found].players     = sqlite3_column_int(stmt, 5);
                dest[found].max_players = sqlite3_column_int(stmt, 6);
                dest[found].status      = sqlite3_column_int(stmt, 7);
                dest[found].extra       = sqlite3_column_int(stmt, 8);
                dest[found].brackets    = sqlite3_column_int(stmt, 9);

                found += 1;
        }

        sqlite_ok_or_log(sqlite3_finalize(stmt));
        conn_close();

        return found;
}

size_t
storage_get_characters(struct gs_character *dest, char *username, size_t max)
{
        sqlite3_stmt *stmt = 0;

        size_t found = 0;

        assert(dest);
        assert(username);

        conn_open();

        sqlite_query(stmt, CHARACTERS_BY_USERNAME_QUERY);

        sqlite_bind_text(stmt, ":username", username);
        sqlite_bind_int(stmt, ":limit", (int) max);

        while (found < max && sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite_to_character(&dest[found], stmt);
                found += 1;
        }

        sqlite_ok_or_log(sqlite3_finalize(stmt));
        conn_close();

        return found;
}

void storage_get_character(
        struct gs_character *dest,
        char *username,
        size_t index)
{
        sqlite3_stmt *stmt = 0;

        assert(dest);
        assert(username);

        conn_open();

        sqlite_query(stmt, CHARACTERS_BY_USERNAME_AND_INDEX_QUERY);

        sqlite_bind_text(stmt, ":username", username);
        sqlite_bind_int(stmt, ":index", (int) index);

        sqlite_ok_or_log(sqlite3_step(stmt));
        sqlite_to_character(dest, stmt);
        sqlite_ok_or_log(sqlite3_finalize(stmt));

        conn_close();
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
        sqlite_bind_int(stmt, ":_int", src->stats._int);
        sqlite_bind_int(stmt, ":str", src->stats.str);
        sqlite_bind_int(stmt, ":con", src->stats.con);
        sqlite_bind_int(stmt, ":men", src->stats.men);
        sqlite_bind_int(stmt, ":dex", src->stats.dex);
        sqlite_bind_int(stmt, ":wit", src->stats.wit);
        bind_int_attr(stmt, src, hair_style);
        bind_int_attr(stmt, src, hair_color);
        bind_int_attr(stmt, src, face);
        bind_int_attr(stmt, src, level);
        bind_int_attr(stmt, src, exp);
        bind_int_attr(stmt, src, sp);
        sqlite_bind_int(stmt, ":hp", src->stats.hp);
        sqlite_bind_int(stmt, ":mp", src->stats.mp);
        sqlite_bind_int(stmt, ":max_hp", src->stats.max_hp);
        sqlite_bind_int(stmt, ":max_mp", src->stats.max_mp);
        sqlite_bind_int(stmt, ":max_cp", src->stats.max_cp);
        sqlite_bind_int(stmt, ":p_attack", src->stats.p_attack);
        sqlite_bind_int(stmt, ":m_attack", src->stats.m_attack);
        sqlite_bind_int(stmt, ":p_def", src->stats.p_def);
        sqlite_bind_int(stmt, ":m_def", src->stats.m_def);
        sqlite_bind_int(stmt, ":evasion_rate", src->stats.evasion_rate);
        sqlite_bind_int(stmt, ":accuracy", src->stats.accuracy);
        sqlite_bind_int(stmt, ":critical_hit", src->stats.critical_hit);
        sqlite_bind_int(stmt, ":run_speed", src->stats.run_speed);
        sqlite_bind_int(stmt, ":walk_speed", src->stats.walk_speed);
        sqlite_bind_int(stmt, ":p_attack_speed", src->stats.p_attack_speed);
        sqlite_bind_int(stmt, ":m_attack_speed", src->stats.m_attack_speed);
        sqlite_bind_int(
                stmt,
                ":movement_speed_multiplier",
                src->stats.movement_speed_multiplier);
        sqlite_bind_int(
                stmt,
                ":attack_speed_multiplier",
                src->stats.attack_speed_multiplier);
        bind_int_attr(stmt, src, collision_radius);
        bind_int_attr(stmt, src, collision_height);
        bind_int_attr(stmt, src, name_color);
        bind_int_attr(stmt, src, max_load);
        sqlite_bind_int(stmt, ":x", src->position.x);
        sqlite_bind_int(stmt, ":y", src->position.y);
        sqlite_bind_int(stmt, ":z", src->position.z);

        sqlite_step_and_finalize(stmt);

        result = sqlite_last_op_ok();

        conn_close();

        return result;
}
