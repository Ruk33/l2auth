#ifndef STORAGE_H
#define STORAGE_H

#include "l2auth.h"

// accounts
int storage_get_account(struct account *dest, struct username *username);
int storage_create_account(struct username *username, struct password *password);
int storage_create_character(struct username *username, struct l2_character *src);
int storage_get_characters(struct l2_character *dest, struct username *username, int max, int *found);
int storage_get_character(struct l2_character *dest, struct username *username, int index);

// servers
int storage_get_servers(struct server *dest, u8 *found, size_t max);
int storage_create_server(struct server *src);

#endif
