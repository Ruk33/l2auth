#ifndef DB_H
#define DB_H

#include "types.h"

// Get characters from account.
// Returns number of characters found.
size_t db_get_account_characters(struct character *dest, struct username *username, size_t max);

#endif
