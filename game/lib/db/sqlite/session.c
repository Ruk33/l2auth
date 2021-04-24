#include <headers.h>
#include <sqlite3.h>
#include "helper.h"
#include "../conn.h"
#include "../session.h"

/**
 * NOTE: Need a way to store logs in case anything goes wrong.
 * NOTE: Instead of accepting just the db_conn_t, it could receive
 * the entire server struct, where not only it can access the db
 * connection but also a handler for logs
 */

void db_session_add(db_conn_t *db, int session_id, session_t *session)
{
        char query[] = "insert into sessions ("
                       "id, username, encrypt_key, decrypt_key,"
                       "playOK1, playOK2, loginOK1, loginOK2,"
                       "char_index, encrypted, state "
                       ") values ("
                       ":id, :username, :encrypt_key, :decrypt_key,"
                       ":playOK1, :playOK2, :loginOK1, :loginOK2,"
                       ":char_index, :encrypted, :state"
                       ")";

        sqlite3_stmt *stmt = NULL;

        int selected_char = 0;

        assert(db);
        assert(session);

        selected_char = (int) session->selected_character_index;

        sqlite3_prepare_v2(db, query, sizeof(query) - 1, &stmt, NULL);

        /**
         * NOTE: Not sure if all this information needs
         * to be saved (ie playOK & loginOK).
         */
        helper_bind_int(stmt, ":id", session_id);
        helper_bind_text(stmt, ":username", session->username);
        helper_bind_blob_arr(stmt, ":encrypt_key", session->encrypt_key);
        helper_bind_blob_arr(stmt, ":decrypt_key", session->decrypt_key);
        helper_bind_int(stmt, ":playOK1", session->playOK1);
        helper_bind_int(stmt, ":playOK2", session->playOK2);
        helper_bind_int(stmt, ":loginOK1", session->loginOK1);
        helper_bind_int(stmt, ":loginOK2", session->loginOK2);
        helper_bind_int(stmt, ":char_index", selected_char);
        helper_bind_int(stmt, ":encrypted", session->conn_encrypted);
        helper_bind_int(stmt, ":state", session->state);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
}

void db_session_remove(db_conn_t *db, int session_id)
{
        char *query_fmt = "delete from sessions where id = %d limit 1;";
        char  query[64] = { 0 };

        assert(db);

        // Note: We may want to use sqlite bind functions instead
        // of "manually" binding the id to the query
        // but since it isn't a complex query/value, it should be fine.
        snprintf(query, sizeof(query), query_fmt, session_id);
        sqlite3_exec(db, query, NULL, NULL, NULL);
}

int db_session_get(db_conn_t *db, session_t *dest, int session_id)
{
        char query[] = "select "
                       "id, username, encrypt_key, decrypt_key,"
                       "playOK1, playOK2, loginOK1, loginOK2,"
                       "char_index, encrypted, state "
                       "from sessions where id = :id limit 1;";

        sqlite3_stmt *stmt = NULL;

        assert(db);
        assert(dest);

        sqlite3_prepare_v2(db, query, sizeof(query) - 1, &stmt, NULL);
        helper_bind_int(stmt, ":id", session_id);

        if (sqlite3_step(stmt) != SQLITE_ROW) {
                sqlite3_finalize(stmt);
                db_conn_close(db);
                return 0;
        }

        /**
         * TODO: Refactor, don't like these magic index numbers...
         * TODO: Saving the socket can't be a good idea...
         */
        dest->socket = sqlite3_column_int(stmt, 0);
        strncpy(dest->username,
                (char *) sqlite3_column_text(stmt, 1),
                sizeof(dest->username));
        memcpy(dest->encrypt_key,
               sqlite3_column_blob(stmt, 2),
               sizeof(dest->encrypt_key));
        memcpy(dest->decrypt_key,
               sqlite3_column_blob(stmt, 3),
               sizeof(dest->decrypt_key));
        dest->playOK1  = sqlite3_column_int(stmt, 4);
        dest->playOK2  = sqlite3_column_int(stmt, 5);
        dest->loginOK1 = sqlite3_column_int(stmt, 6);
        dest->loginOK2 = sqlite3_column_int(stmt, 7);
        dest->selected_character_index =
                (unsigned int) sqlite3_column_int(stmt, 8);
        dest->conn_encrypted = sqlite3_column_int(stmt, 9);
        dest->state          = sqlite3_column_int(stmt, 10);

        sqlite3_finalize(stmt);

        return 1;
}

void db_session_update(db_conn_t *db, int session_id, session_t *src)
{
        char query[] = "update sessions set "
                       "username = :username, encrypt_key = :encrypt_key, "
                       "decrypt_key = :decrypt_key, playOK1 = :playOK1, "
                       "playOK2 = :playOK2, loginOK1 = :loginOK1, "
                       "loginOK2 = :loginOK2, char_index = :char_index, "
                       "encrypted = :encrypted, state = :state "
                       "where id = :id;";

        sqlite3_stmt *stmt = NULL;

        int char_index = 0;

        assert(db);
        assert(src);

        char_index = (int) src->selected_character_index;

        sqlite3_prepare(db, query, sizeof(query) - 1, &stmt, NULL);

        helper_bind_text(stmt, ":username", src->username);
        helper_bind_blob_arr(stmt, ":encrypt_key", src->encrypt_key);
        helper_bind_blob_arr(stmt, ":decrypt_key", src->decrypt_key);
        helper_bind_int(stmt, ":playOK1", src->playOK1);
        helper_bind_int(stmt, ":playOK2", src->playOK2);
        helper_bind_int(stmt, ":loginOK1", src->loginOK1);
        helper_bind_int(stmt, ":loginOK2", src->loginOK2);
        helper_bind_int(stmt, ":char_index", char_index);
        helper_bind_int(stmt, ":encrypted", src->conn_encrypted);
        helper_bind_int(stmt, ":state", src->state);
        helper_bind_int(stmt, ":id", session_id);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
}
