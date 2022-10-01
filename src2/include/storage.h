#ifndef STORAGE_H
#define STORAGE_H

#include "l2auth.h"

// 0 if not found
// 1 if found
// dest can be null
int storage_get_account(struct account *dest, struct username *username);

int storage_create_account(struct username *username, struct password *password);

#endif
