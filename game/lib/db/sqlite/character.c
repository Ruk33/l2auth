#include <headers.h>
#include <sqlite3.h>
#include <character.h>
#include "helper.h"
#include "../conn.h"
#include "../character.h"

static void bind_character(sqlite3_stmt *stmt, character_t *character)
{
        assert(stmt);
        assert(character);

        helper_bind_int(stmt, ":id", character->id);
        helper_bind_int(stmt, ":x", character->x);
        helper_bind_int(stmt, ":y", character->y);
        helper_bind_int(stmt, ":z", character->z);
}

static int sqlite_to_character(void *p, int col_count, char **cell, char **col)
{
        character_t *s = NULL;

        s = (character_t *) p;

        if (s && cell) {
        }

        for (int i = 0; i < col_count; i++) {
                if (strcmp("username", col[i]) == 0) {
                }
        }

        return 0;
}

char_obj_id_t db_character_add(db_conn_t *db, character_t *character)
{
        char query[] =
                "insert into characters (id, x, y, z) values (:id, :x, :y, :z);";

        sqlite3_stmt *stmt = NULL;

        assert(db);
        assert(character);

        sqlite3_prepare(db, query, sizeof(query) - 1, &stmt, NULL);

        bind_character(stmt, character);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        /**
         * NOTE: Not sure if this will return the expected result.
         */
        return (char_obj_id_t) sqlite3_last_insert_rowid(db);
}

void db_character_get(db_conn_t *db, character_t *dest, char_obj_id_t id)
{
        char *query_fmt = "select * from characters where id = %d limit 1;";
        char  query[64] = { 0 };

        assert(db);
        assert(dest);

        // Note: We may want to use sqlite bind functions instead
        // of "manually" binding the id to the query
        // but since it isn't a complex query/value, it should be fine.
        snprintf(query, sizeof(query), query_fmt, id);
        sqlite3_exec(db, query, &sqlite_to_character, dest, NULL);
}

void db_character_remove(db_conn_t *db, char_obj_id_t id)
{
        char *query_fmt = "delete from characters where id = %d limit 1;";
        char  query[64] = { 0 };

        assert(db);

        // Note: We may want to use sqlite bind functions instead
        // of "manually" binding the id to the query
        // but since it isn't a complex query/value, it should be fine.
        snprintf(query, sizeof(query), query_fmt, id);
        sqlite3_exec(db, query, NULL, NULL, NULL);
}

void db_character_update(db_conn_t *db, char_obj_id_t id, character_t *src)
{
        char query[] =
                "update characters set x = :x, y = :y, z = :z where obj_id = :obj_id;";

        sqlite3_stmt *stmt = NULL;

        assert(db);
        assert(src);

        sqlite3_prepare(db, query, sizeof(query) - 1, &stmt, NULL);

        helper_bind_int(stmt, ":obj_id", (int) id);
        bind_character(stmt, src);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
}

size_t db_character_in_radius(
        db_conn_t *  db,
        character_t *dest,
        size_t       max,
        position_t * pos,
        int          radius)
{
        assert(db);
        assert(dest);
        assert(pos);
        if (max && radius) {
        }
        return 0;
}
