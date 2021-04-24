#include <headers.h>
#include <db/conn.h>
#include <db/session.h>
#include <session.h>
#include "session_crypt.h"

void util_session_encrypt_packet(
        db_conn_t *db,
        int        session_id,
        byte_t *   dest,
        packet *   src,
        size_t     src_size)
{
        session_t session = { 0 };

        assert(db);
        assert(dest);
        assert(src);

        db_session_get(db, &session, session_id);
        session_encrypt_packet(&session, dest, src, src_size);
        db_session_update(db, session_id, &session);
}

void util_session_decrypt_packet(
        db_conn_t *db,
        int        session_id,
        packet *   dest,
        packet *   src,
        size_t     src_size)
{
        session_t session = { 0 };

        assert(db);
        assert(dest);
        assert(src);

        db_session_get(db, &session, session_id);
        session_decrypt_packet(&session, dest, src, src_size);
        db_session_update(db, session_id, &session);
}

void util_session_encrypt_connection(db_conn_t *db, int session_id)
{
        session_t session = { 0 };
        db_session_get(db, &session, session_id);
        session.conn_encrypted = 1;
        db_session_update(db, session_id, &session);
}
