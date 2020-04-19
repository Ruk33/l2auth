#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <log/log.h>
#include <game/db/conn.h>
#include <game/dto/char.h>
#include <game/db/char.h>

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

void game_db_char_create_table_if_required(game_db_conn* db)
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

void game_db_char_save(game_db_conn* db, struct GameDtoChar* character)
{
        assert(db);
        assert(character);

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

        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_ID, character->char_id);
        sqlite3_bind_text(stmt, GAME_DB_CHAR_COL_NAME, character->name, -1, NULL);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_RACE_ID, character->race_id);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_SEX, character->sex);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_CLASS_ID, character->class_id);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_INT, character->_int);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_STR, character->str);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_CON, character->con);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_MEN, character->men);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_DEX, character->dex);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_WIT, character->wit);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_HAIR_STYLE_ID, character->hair_style);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_HAIR_COLOR_ID, character->hair_color);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_FACE_ID, character->face);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_X, character->current_location.x);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_Y, character->current_location.y);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_Z, character->current_location.z);
        sqlite3_bind_double(stmt, GAME_DB_CHAR_COL_MAX_HP, character->max_hp);
        sqlite3_bind_double(stmt, GAME_DB_CHAR_COL_MAX_MP, character->max_mp);
        sqlite3_bind_double(stmt, GAME_DB_CHAR_COL_HP, character->current_hp);
        sqlite3_bind_double(stmt, GAME_DB_CHAR_COL_MP, character->current_mp);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_ACTIVE, character->active);
        sqlite3_bind_int(stmt, GAME_DB_CHAR_COL_LEVEL, character->level);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
}

int game_db_char_list
(
        game_db_conn* db,
        struct GameDtoChar** characters,
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
                LIMIT ?",
                -1,
                &stmt,
                NULL
        );

        sqlite3_bind_int(stmt, 1, (int) how_many);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
                characters[count]->char_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_ID - 1);
                memset(characters[count]->name, 0, sizeof(characters[count]->name));
                strcat(characters[count]->name, (const char *) sqlite3_column_text(stmt, GAME_DB_CHAR_COL_NAME - 1));
                characters[count]->race_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_RACE_ID - 1);
                characters[count]->sex = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_SEX - 1);
                characters[count]->class_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_CLASS_ID - 1);
                characters[count]->_int = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_INT - 1);
                characters[count]->str = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_STR - 1);
                characters[count]->con = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_CON - 1);
                characters[count]->men = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_MEN - 1);
                characters[count]->dex = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_DEX - 1);
                characters[count]->wit = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_WIT - 1);
                characters[count]->hair_style = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_HAIR_STYLE_ID - 1);
                characters[count]->hair_color = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_HAIR_COLOR_ID - 1);
                characters[count]->face = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_FACE_ID - 1);
                characters[count]->current_location.x = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_X - 1);
                characters[count]->current_location.y = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_Y - 1);
                characters[count]->current_location.z = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_Z - 1);
                characters[count]->max_hp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MAX_HP - 1);
                characters[count]->max_mp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MAX_MP - 1);
                characters[count]->current_hp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_HP - 1);
                characters[count]->current_mp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MP - 1);
                characters[count]->active = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_ACTIVE - 1);
                characters[count]->level = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_LEVEL - 1);

                count++;
        }

        sqlite3_finalize(stmt);

        return count;
}

void game_db_char_get_from_index
(
        game_db_conn* db,
        struct GameDtoChar* character
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
                LIMIT 1",
                -1,
                &stmt,
                NULL
        );

        while (sqlite3_step(stmt) == SQLITE_ROW) {
                character->char_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_ID - 1);
                memset(character->name, 0, sizeof(character->name));
                strcat(character->name, (const char *) sqlite3_column_text(stmt, GAME_DB_CHAR_COL_NAME - 1));
                character->race_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_RACE_ID - 1);
                character->sex = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_SEX - 1);
                character->class_id = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_CLASS_ID - 1);
                character->_int = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_INT - 1);
                character->str = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_STR - 1);
                character->con = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_CON - 1);
                character->men = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_MEN - 1);
                character->dex = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_DEX - 1);
                character->wit = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_WIT - 1);
                character->hair_style = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_HAIR_STYLE_ID - 1);
                character->hair_color = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_HAIR_COLOR_ID - 1);
                character->face = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_FACE_ID - 1);
                character->current_location.x = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_X - 1);
                character->current_location.y = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_Y - 1);
                character->current_location.z = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_Z - 1);
                character->max_hp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MAX_HP - 1);
                character->max_mp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MAX_MP - 1);
                character->current_hp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_HP - 1);
                character->current_mp = sqlite3_column_double(stmt, GAME_DB_CHAR_COL_MP - 1);
                character->active = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_ACTIVE - 1);
                character->level = sqlite3_column_int(stmt, GAME_DB_CHAR_COL_LEVEL - 1);
        }

        sqlite3_finalize(stmt);
}
