#ifndef L2AUTH_GAME_DB_CONN_H
#define L2AUTH_GAME_DB_CONN_H

typedef void game_db_conn;

game_db_conn* game_db_conn_open();
void game_db_conn_close(game_db_conn* db);

#endif
