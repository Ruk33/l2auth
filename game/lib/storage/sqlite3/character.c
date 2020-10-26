#include <assert.h>
#include <string.h>
#include <sqlite3.h>
#include <log/log.h>
#include "../../client.h"
#include "../../entity/player.h"
#include "../conn.h"

/*
 * NOTE, keep CHARACTER_TABLE_COLUMNS & CHARACTER_TABLE_COLUMN_KEYS in order
 * to make sqlite queries easier to write
 */

#define CHARACTER_TABLE_COLUMNS \
        "                                               \
        id, name, race_id, sex, class_id, _int,         \
        str, con, men, dex, wit, hair_style_id,         \
        hair_color_id, face_id, x, y, z, max_hp,        \
        max_mp, hp, mp, active, level                   \
        "

static enum CHARACTER_TABLE_COLUMN_KEYS {
        CHARACTER_ID,
        CHARACTER_NAME,
        CHARACTER_RACE_ID,
        CHARACTER_SEX,
        CHARACTER_CLASS_ID,
        CHARACTER_INT,
        CHARACTER_STR,
        CHARACTER_CON,
        CHARACTER_MEN,
        CHARACTER_DEX,
        CHARACTER_WIT,
        CHARACTER_HAIR_STYLE_ID,
        CHARACTER_HAIR_COLOR_ID,
        CHARACTER_FACE_ID,
        CHARACTER_X,
        CHARACTER_Y,
        CHARACTER_Z,
        CHARACTER_MAX_HP,
        CHARACTER_MAX_MP,
        CHARACTER_HP,
        CHARACTER_MP,
        CHARACTER_ACTIVE,
        CHARACTER_LEVEL
};

static void create_table_if_required(conn_handler *conn)
{
        assert(conn);

        sqlite3_exec(
            (sqlite3 *)conn,
            "CREATE TABLE IF NOT EXISTS characters ( \
                        id INT PRIMARY KEY, \
                        name TEXT, \
                        race_id INT, \
                        sex INT, \
                        class_id INT, \
                        _int INT, \
                        str INT, \
                        con INT, \
                        men INT, \
                        dex INT, \
                        wit INT, \
                        hair_style_id INT, \
                        hair_color_id INT, \
                        face_id INT, \
                        x INT, \
                        y INT, \
                        z INT, \
                        max_hp DOUBLE, \
                        max_mp DOUBLE, \
                        hp DOUBLE, \
                        mp DOUBLE, \
                        active INT, \
                        level INT \
                )",
            NULL,
            NULL,
            NULL);
}

static void query_to_char(sqlite3_stmt *stmt, struct Player *player)
{
        assert(stmt);
        assert(player);

        player->character.id = sqlite3_column_int(stmt, CHARACTER_ID);

        memset(player->character.name, 0, sizeof(player->character.name));
        strcat(
            player->character.name,
            (const char *)sqlite3_column_text(stmt, CHARACTER_NAME));

        player->race_id = sqlite3_column_int(stmt, CHARACTER_RACE_ID);
        player->character.sex = sqlite3_column_int(stmt, CHARACTER_SEX);
        player->class_id = sqlite3_column_int(stmt, CHARACTER_CLASS_ID);
        player->character._int = sqlite3_column_int(stmt, CHARACTER_INT);
        player->character.str = sqlite3_column_int(stmt, CHARACTER_STR);
        player->character.con = sqlite3_column_int(stmt, CHARACTER_CON);
        player->character.men = sqlite3_column_int(stmt, CHARACTER_MEN);
        player->character.dex = sqlite3_column_int(stmt, CHARACTER_DEX);
        player->character.wit = sqlite3_column_int(stmt, CHARACTER_WIT);
        player->hair_style_id = sqlite3_column_int(stmt, CHARACTER_HAIR_STYLE_ID);
        player->hair_color_id = sqlite3_column_int(stmt, CHARACTER_HAIR_COLOR_ID);
        player->face = sqlite3_column_int(stmt, CHARACTER_FACE_ID);
        player->character.x = sqlite3_column_int(stmt, CHARACTER_X);
        player->character.y = sqlite3_column_int(stmt, CHARACTER_Y);
        player->character.z = sqlite3_column_int(stmt, CHARACTER_Z);
        player->character.hp = sqlite3_column_double(stmt, CHARACTER_MAX_HP);
        player->character.mp = sqlite3_column_double(stmt, CHARACTER_MAX_MP);
        player->character.current_hp = sqlite3_column_double(stmt, CHARACTER_HP);
        player->character.current_mp = sqlite3_column_double(stmt, CHARACTER_MP);
        player->active = sqlite3_column_int(stmt, CHARACTER_ACTIVE);
        player->character.level = sqlite3_column_int(stmt, CHARACTER_LEVEL);
}

struct Player **storage_characters_all(struct Client *client)
{
        assert(client);

        size_t max_chars = 5;
        struct Player **characters = NULL;

        conn_handler *conn = NULL;
        sqlite3_stmt *stmt = NULL;

        characters = client_alloc_mem(client, sizeof(*characters) * max_chars);
        conn = conn_open();

        create_table_if_required(conn);

        sqlite3_prepare_v2(
            (sqlite3 *)conn,
            "SELECT " CHARACTER_TABLE_COLUMNS "\
                FROM characters \
                ORDER BY name DESC \
                LIMIT ?",
            -1,
            &stmt,
            NULL);

        sqlite3_bind_int(stmt, 1, (int)max_chars);

        for (int i = 0; sqlite3_step(stmt) == SQLITE_ROW; i++)
        {
                characters[i] = client_alloc_mem(client, sizeof(struct Player));
                query_to_char(stmt, characters[i]);
        }

        sqlite3_finalize(stmt);
        conn_close(conn);

        return characters;
}

struct Player *storage_character_get(struct Client *client, int index)
{
        assert(client);

        struct Player *player = NULL;
        conn_handler *conn = NULL;
        sqlite3_stmt *stmt = NULL;

        player = client_alloc_mem(client, sizeof(*player));
        conn = conn_open();

        create_table_if_required(conn);

        sqlite3_prepare_v2(
            (sqlite3 *)conn,
            "SELECT " CHARACTER_TABLE_COLUMNS "\
                FROM characters \
                ORDER BY name DESC \
                LIMIT 1 \
                OFFSET ?",
            -1,
            &stmt,
            NULL);

        sqlite3_bind_int(stmt, 1, index);

        sqlite3_step(stmt);
        query_to_char(stmt, player);
        sqlite3_finalize(stmt);

        conn_close(conn);

        return player;
}

void storage_character_save(struct Client *client, struct Player *player)
{
        assert(client);
        assert(player);

        conn_handler *conn = NULL;
        sqlite3_stmt *stmt = NULL;
        int result = 0;

        conn = conn_open();

        create_table_if_required(conn);

        result = sqlite3_prepare_v2(
            (sqlite3 *)conn,
            "INSERT OR REPLACE INTO characters \
                (" CHARACTER_TABLE_COLUMNS ") \
                VALUES \
                ( \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ?, \
                        ? \
                )",
            -1,
            &stmt,
            NULL);

        sqlite3_bind_int(stmt, CHARACTER_ID + 1, player->character.id);
        sqlite3_bind_text(stmt, CHARACTER_NAME + 1, player->character.name, -1, NULL);
        sqlite3_bind_int(stmt, CHARACTER_RACE_ID + 1, player->race_id);
        sqlite3_bind_int(stmt, CHARACTER_SEX + 1, player->character.sex);
        sqlite3_bind_int(stmt, CHARACTER_CLASS_ID + 1, player->class_id);
        sqlite3_bind_int(stmt, CHARACTER_INT + 1, player->character._int);
        sqlite3_bind_int(stmt, CHARACTER_STR + 1, player->character.str);
        sqlite3_bind_int(stmt, CHARACTER_CON + 1, player->character.con);
        sqlite3_bind_int(stmt, CHARACTER_MEN + 1, player->character.men);
        sqlite3_bind_int(stmt, CHARACTER_DEX + 1, player->character.dex);
        sqlite3_bind_int(stmt, CHARACTER_WIT + 1, player->character.wit);
        sqlite3_bind_int(stmt, CHARACTER_HAIR_STYLE_ID + 1, player->hair_style_id);
        sqlite3_bind_int(stmt, CHARACTER_HAIR_COLOR_ID + 1, player->hair_color_id);
        sqlite3_bind_int(stmt, CHARACTER_FACE_ID + 1, player->face);
        sqlite3_bind_int(stmt, CHARACTER_X + 1, player->character.x);
        sqlite3_bind_int(stmt, CHARACTER_Y + 1, player->character.y);
        sqlite3_bind_int(stmt, CHARACTER_Z + 1, player->character.z);
        sqlite3_bind_double(stmt, CHARACTER_MAX_HP + 1, player->character.hp);
        sqlite3_bind_double(stmt, CHARACTER_MAX_MP + 1, player->character.mp);
        sqlite3_bind_double(stmt, CHARACTER_HP + 1, player->character.current_hp);
        sqlite3_bind_double(stmt, CHARACTER_MP + 1, player->character.current_mp);
        sqlite3_bind_int(stmt, CHARACTER_ACTIVE + 1, player->active);
        sqlite3_bind_int(stmt, CHARACTER_LEVEL + 1, player->character.level);

        result = sqlite3_step(stmt);

        if (result != SQLITE_OK)
        {
                log_fatal(sqlite3_errmsg((sqlite3 *)conn));
        }

        result = sqlite3_finalize(stmt);

        if (result != SQLITE_OK)
        {
                log_fatal(sqlite3_errmsg((sqlite3 *)conn));
        }

        conn_close(conn);
}
