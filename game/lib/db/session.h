#ifndef LIB_DB_SESSION_H
#define LIB_DB_SESSION_H

#include <headers.h>
#include <db/conn.h>
#include <session.h>

/**
 * Register a new session.
 */
void db_session_add(db_conn_t *db, int session_id, session_t *session);

/**
 * Remove session.
 */
void db_session_remove(db_conn_t *db, int session_id);

/**
 * Get session by id.
 * If found, store on dest and return 1.
 * If not found, return 0.
 */
int db_session_get(db_conn_t *db, session_t *dest, int session_id);

/**
 * Update session with src values.
 */
void db_session_update(db_conn_t *db, int session_id, session_t *src);

#endif
