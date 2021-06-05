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
        sqlite3_bind_text(                      \
                stmt, sqlite3_bind_parameter_index(stmt, key), value, n, NULL)

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
#define sqlite_bind_int(stmt, key, value) \
        sqlite3_bind_int(stmt, sqlite3_bind_parameter_index(stmt, key), value)

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

#define CHARACTERS_COLUMNS \
        "username, name, race, sex, _class, _int, str, con, men, dex, wit, hair_style, hair_color, face"

#define CHARACTERS_BY_USERNAME_QUERY \
        "select " CHARACTERS_COLUMNS \
        " from characters where username = :username limit :limit;"

#define CHARACTERS_CREATE_QUERY                       \
        "insert into characters (" CHARACTERS_COLUMNS \
        ") values (:username, :name, :race, :sex, :_class, :_int, :str, :con, :men, :dex, :wit, :hair_style, :hair_color, :face);"

static sqlite3 *conn = 0;

static void sqlite_to_character(gs_character_t *dest, sqlite3_stmt *stmt)
{
        sqlite_cpy_text((byte_t *) (dest->name), stmt, 1);
        dest->race       = sqlite3_column_int(stmt, 2);
        dest->sex        = sqlite3_column_int(stmt, 3);
        dest->_class     = sqlite3_column_int(stmt, 4);
        dest->_int       = sqlite3_column_int(stmt, 5);
        dest->str        = sqlite3_column_int(stmt, 6);
        dest->con        = sqlite3_column_int(stmt, 7);
        dest->men        = sqlite3_column_int(stmt, 8);
        dest->dex        = sqlite3_column_int(stmt, 9);
        dest->wit        = sqlite3_column_int(stmt, 10);
        dest->hair_style = sqlite3_column_int(stmt, 11);
        dest->hair_color = sqlite3_column_int(stmt, 12);
        dest->face       = sqlite3_column_int(stmt, 13);
}

void storage_open(void)
{
        sqlite_ok_or_log(sqlite3_open("db", &conn));
}

void storage_close(void)
{
        sqlite3_close(conn);
}

size_t storage_get_characters(gs_character_t *dest, char *username, size_t max)
{
        sqlite3_stmt *stmt = 0;

        size_t found = 0;

        sqlite_query(stmt, CHARACTERS_BY_USERNAME_QUERY);

        sqlite_ok_or_log(sqlite_bind_text(stmt, ":username", username));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":limit", (int) max));

        while (found < max && sqlite3_step(stmt) == SQLITE_ROW) {
                sqlite_to_character(&dest[found], stmt);
                found += 1;
        }

        return found;
}

int storage_create_character(char *username, gs_character_t *src)
{
        sqlite3_stmt *stmt = 0;

        sqlite_query(stmt, CHARACTERS_CREATE_QUERY);

        sqlite_ok_or_log(sqlite_bind_text(stmt, ":username", username));
        sqlite_ok_or_log(sqlite_bind_text(stmt, ":name", src->name));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":race", src->race));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":sex", src->sex));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":_class", src->_class));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":_int", src->_int));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":str", src->str));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":con", src->con));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":men", src->men));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":dex", src->dex));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":wit", src->wit));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":hair_style", src->hair_style));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":hair_color", src->hair_color));
        sqlite_ok_or_log(sqlite_bind_int(stmt, ":face", src->face));

        sqlite_step_and_finalize(stmt);

        return sqlite_last_op_ok();
}
