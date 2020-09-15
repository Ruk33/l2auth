#ifndef STORAGE_CLIENT_H
#define STORAGE_CLIENT_H

#include "conn.h"
#include "../client.h"
#include "../entity/pc.h"

struct Pc **storage_characters_all
(conn_handler *conn, struct Client *client);

struct Pc *storage_character_get
(conn_handler *conn, struct Client *client, int index);

void storage_character_save
(conn_handler *conn, struct Client *client, struct Pc *character);

#endif
