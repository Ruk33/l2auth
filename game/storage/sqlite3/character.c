#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <log/log.h>
#include <game/entity/player.h>
#include "../connection.h"
#include "../character.h"

enum game_db_char_col {
        GAME_DB_CHAR_COL_EMPTY,
        GAME_DB_CHAR_COL_ID,
        GAME_DB_CHAR_COL_NAME,
        GAME_DB_CHAR_COL_RACE_ID,
        GAME_DB_CHAR_COL_SEX,
        GAME_DB_CHAR_COL_CLASS_ID,
        GAME_DB_CHAR_COL_INT,
        GAME_DB_CHAR_COL_STR,
        GAME_DB_CHAR_COL_CON,
        GAME_DB_CHAR_COL_MEN,
        GAME_DB_CHAR_COL_DEX,
        GAME_DB_CHAR_COL_WIT,
        GAME_DB_CHAR_COL_HAIR_STYLE_ID,
        GAME_DB_CHAR_COL_HAIR_COLOR_ID,
        GAME_DB_CHAR_COL_FACE_ID,
        GAME_DB_CHAR_COL_X,
        GAME_DB_CHAR_COL_Y,
        GAME_DB_CHAR_COL_Z,
        GAME_DB_CHAR_COL_MAX_HP,
        GAME_DB_CHAR_COL_MAX_MP,
        GAME_DB_CHAR_COL_HP,
        GAME_DB_CHAR_COL_MP,
        GAME_DB_CHAR_COL_ACTIVE,
        GAME_DB_CHAR_COL_LEVEL,
};

void game_db_char_create_table_if_required(game_storage_connection* db)
{
        assert(db);

        char* error;
        sqlite3_exec(
                db,
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
                &error
        );
}

void game_storage_character_save(game_storage_connection* db, struct Player* player)
{
        assert(db);
        assert(player);

        sqlite3_stmt* stmt;

        game_db_char_create_table_if_required(db);

        sqlite3_prepare_v2(
                db,
                "INSERT OR REPLACE INTO characters \
                ( \
                        id, \
                        name, \
                        race_id, \
                        sex, \
                        class_id, \
                        _int, \
                        str, \
                        con, \
                        men, \
                        dex, \
                        wit, \
                        hair_style_id, \
                        hair_color_id, \
                        face_id, \
                        x, \
                        y, \
                        z, \
                        max_hp, \
                        max_mp, \
                        hp, \
                        mp, \
                        active, \
                        level \
                ) \
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
                NULL
        );

        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_ID, player->character.id);
        sqlite3_bind_text(stmt, GAME_DB_CHAR_COL_NAME, player->character.name, -1, NULL);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_RACE_ID, player->race_id);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_SEX, player->character.sex);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_CLASS_ID, player->class_id);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_INT, player->character._int);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_STR, player->character.str);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_CON, player->character.con);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_MEN, player->character.men);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_DEX, player->character.dex);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_WIT, player->character.wit);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_HAIR_STYLE_ID, player->hair_style_id);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_HAIR_COLOR_ID, player->hair_color_id);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_FACE_ID, player->face);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_X, player->character.x);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_Y, player->character.y);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_Z, player->character.z);
        sqlite3_bind_double(stmt, GAME_DB_CHAR_COL_MAX_HP, player->character.hp);
        sqlite3_bind_double(stmt, GAME_DB_CHAR_COL_MAX_MP, player->character.mp);
        sqlite3_bind_double(stmt, GAME_DB_CHAR_COL_HP, player->character.current_hp);
        sqlite3_bind_double(stmt, GAME_DB_CHAR_COL_MP, player->character.current_mp);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_ACTIVE, player->active);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_LEVEL, player->character.level);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
}

int game_storage_character_list
(
        game_storage_connection* db,
        struct Player** players,
        size_t how_many
)
{
        sqlite3_stmt* stmt;
        int count = 0;

        game_db_char_create_table_if_required(db);

        sqlite3_prepare_v2(
                db,
                "SELECT \
                        id, \
                        name, \
                        race_id, \
                        sex, \
                        class_id, \
                        _int, \
                        str, \
                        con, \
                        men, \
                        dex, \
                        wit, \
                        hair_style_id, \
                        hair_color_id, \
                        face_id, \
                        x, \
                        y, \
                        z, \
                        max_hp, \
                        max_mp, \
                        hp, \
                        mp, \
                        active, \
                        level \
                FROM characters \
                ORDER BY name DESC \
                LIMIT ?",
                -1,
                &stmt,
                NULL
        );

        sqlite3_bind_int(stmt, 1, (int) how_many);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
                players[count]->character.id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_ID - 1);
                memset(players[count]->character.name, 0, sizeof(players[count]->character.name));
                strcat(players[count]->character.name, (const char *) sqlite3_column_text(stmt, GAME_DB_CHAR_COL_NAME - 1));
                log_fatal("name from db %s", players[count]->character.name);
                players[count]->race_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_RACE_ID - 1);
                players[count]->character.sex = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_SEX - 1);
                players[count]->class_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_CLASS_ID - 1);
                players[count]->character._int = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_INT - 1);
                players[count]->character.str = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_STR - 1);
                players[count]->character.con = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_CON - 1);
                players[count]->character.men = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_MEN - 1);
                players[count]->character.dex = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_DEX - 1);
                players[count]->character.wit = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_WIT - 1);
                players[count]->hair_style_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_HAIR_STYLE_ID - 1);
                players[count]->hair_color_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_HAIR_COLOR_ID - 1);
                players[count]->face = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_FACE_ID - 1);
                players[count]->character.x = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_X - 1);
                players[count]->character.y = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_Y - 1);
                players[count]->character.z = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_Z - 1);
                players[count]->character.hp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MAX_HP - 1);
                players[count]->character.mp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MAX_MP - 1);
                players[count]->character.current_hp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_HP - 1);
                players[count]->character.current_mp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MP - 1);
                players[count]->active = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_ACTIVE - 1);
                players[count]->character.level = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_LEVEL - 1);

                count++;
        }

        sqlite3_finalize(stmt);

        return count;
}

void game_storage_character_from_index
(
        game_storage_connection* db,
        struct Player* player,
        int index
)
{
        sqlite3_stmt* stmt;

        game_db_char_create_table_if_required(db);

        sqlite3_prepare_v2(
                db,
                "SELECT \
                        id, \
                        name, \
                        race_id, \
                        sex, \
                        class_id, \
                        _int, \
                        str, \
                        con, \
                        men, \
                        dex, \
                        wit, \
                        hair_style_id, \
                        hair_color_id, \
                        face_id, \
                        x, \
                        y, \
                        z, \
                        max_hp, \
                        max_mp, \
                        hp, \
                        mp, \
                        active, \
                        level \
                FROM characters \
                ORDER BY name DESC \
                LIMIT 1 \
                OFFSET ?",
                -1,
                &stmt,
                NULL
        );

        sqlite3_bind_int(stmt, 1, index);

        sqlite3_step(stmt);

        player->character.id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_ID - 1);
        memset(player->character.name, 0, sizeof(player->character.name));
        strcat(player->character.name, (const char *) sqlite3_column_text(stmt, GAME_DB_CHAR_COL_NAME - 1));
        player->race_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_RACE_ID - 1);
        player->character.sex = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_SEX - 1);
        player->class_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_CLASS_ID - 1);
        player->character._int = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_INT - 1);
        player->character.str = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_STR - 1);
        player->character.con = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_CON - 1);
        player->character.men = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_MEN - 1);
        player->character.dex = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_DEX - 1);
        player->character.wit = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_WIT - 1);
        player->hair_style_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_HAIR_STYLE_ID - 1);
        player->hair_color_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_HAIR_COLOR_ID - 1);
        player->face = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_FACE_ID - 1);
        player->character.x = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_X - 1);
        player->character.y = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_Y - 1);
        player->character.z = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_Z - 1);
        player->character.hp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MAX_HP - 1);
        player->character.mp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MAX_MP - 1);
        player->character.current_hp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_HP - 1);
        player->character.current_mp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MP - 1);
        player->active = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_ACTIVE - 1);
        player->character.level = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_LEVEL - 1);

        sqlite3_finalize(stmt);
}
