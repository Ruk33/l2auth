#ifndef L2AUTH_GAME_STORAGE_CONNECTION_H
#define L2AUTH_GAME_STORAGE_CONNECTION_H

typedef void game_storage_connection;

game_storage_connection* game_storage_connection_open();

void game_storage_connection_close(game_storage_connection* db);

#endif
