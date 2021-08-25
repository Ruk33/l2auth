#ifndef INCLUDE_STORAGE_H
#define INCLUDE_STORAGE_H

#include "util.h"
#include "gs_types.h"
#include "ls_types.h"

// Get account from username and encrypted password.
// Returns 1 if found, 0 otherwise.
int storage_get_account(struct ls_account *dest, char *username);

// Create new account.
// Returns 1 on success, 0 otherwise.
int storage_create_account(struct ls_account *src);

size_t storage_get_servers(struct ls_server *dest, size_t max);

// Get up to max characters owned by username and store it on dest.
// Return the amount of found characters.
size_t
storage_get_characters(struct gs_character *dest, char *username, size_t max);

// Get character using index and store it on dest.
// Used when the user is selecting a character to play with.
void storage_get_character(
        struct gs_character *dest,
        char *username,
        size_t index);

// Save character to database.
// On success, 1 is returned. On error, 0.
int storage_create_character(char *username, struct gs_character *src);

#endif
