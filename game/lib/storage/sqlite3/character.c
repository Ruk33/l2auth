#include <string.h>
#include <sqlite3.h>
#include <log/log.h>
#include "../../client.h"
#include "../../entity/pc.h"
#include "../conn.h"

static void query_to_char(sqlite3_stmt *stmt, struct Pc *player)
{
        player->character.id = sqlite3_column_int(stmt, 0);
        strcat(player->character.name, (const char *) sqlite3_column_text(stmt, 1));
        player->race_id = sqlite3_column_int(stmt, 2);
        player->character.sex = sqlite3_column_int(stmt, 3);
        player->class_id = sqlite3_column_int(stmt, 4);
        player->character._int = sqlite3_column_int(stmt, 5);
        player->character.str = sqlite3_column_int(stmt, 6);
        player->character.con = sqlite3_column_int(stmt, 7);
        player->character.men = sqlite3_column_int(stmt, 8);
        player->character.dex = sqlite3_column_int(stmt, 9);
        player->character.wit = sqlite3_column_int(stmt, 10);
        player->hair_style_id = sqlite3_column_int(stmt, 11);
        player->hair_color_id = sqlite3_column_int(stmt, 12);
        player->face = sqlite3_column_int(stmt, 13);
        player->character.x = sqlite3_column_int(stmt, 14);
        player->character.y = sqlite3_column_int(stmt, 15);
        player->character.z = sqlite3_column_int(stmt, 16);
        player->character.hp = sqlite3_column_double(stmt, 17);
        player->character.mp = sqlite3_column_double(stmt, 18);
        player->character.current_hp = sqlite3_column_double(stmt, 19);
        player->character.current_mp = sqlite3_column_double(stmt, 20);
        player->active = sqlite3_column_int(stmt, 21);
        player->character.level = sqlite3_column_int(stmt, 22);
}

struct Pc **storage_characters_all(conn_handler *conn, struct Client *client)
{
        size_t max_chars = 5;
        struct Pc **characters = client_alloc_mem(client, sizeof(struct Pc *) * max_chars);

        sqlite3 *sqlite;
        sqlite3_stmt *stmt;

        sqlite3_open("gameserver.db", &sqlite);

        // conn_open(conn);

        sqlite3_prepare_v2(
                sqlite,
                "SELECT id, name, race_id, sex, class_id, _int, str, con, men, dex, wit, hair_style_id, hair_color_id, face_id, x, y, z, max_hp, max_mp, hp, mp, active, level \
                FROM characters \
                ORDER BY name DESC \
                LIMIT ?",
                -1,
                &stmt,
                NULL
        );

        sqlite3_bind_int(stmt, 1, (int) max_chars);

        for (int i = 0; sqlite3_step(stmt) == SQLITE_ROW; i++) {
                characters[i] = client_alloc_mem(client, sizeof(struct Pc));
                query_to_char(stmt, characters[i]);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(sqlite);

        // conn_close(sqlite);

        return characters;
}

struct Pc *storage_character_get(conn_handler *conn, struct Client *client, int index)
{
        struct Pc *player = client_alloc_mem(client, sizeof(struct Pc));

        sqlite3 *sqlite;
        sqlite3_stmt *stmt;

        sqlite3_open("gameserver.db", &sqlite);

        sqlite3_prepare_v2(
                sqlite,
                "SELECT id, name, race_id, sex, class_id, _int, str, con, men, dex, wit, hair_style_id, hair_color_id, face_id, x, y, z, max_hp, max_mp, hp, mp, active, level \
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
        query_to_char(stmt, player);

        sqlite3_finalize(stmt);
        sqlite3_close(sqlite);

        return player;
}
