#ifndef INCLUDE_STORAGE_H
#define INCLUDE_STORAGE_H

#include "util.h"
#include "character.h"

void storage_open(void);

// Get up to max characters owned by username and store it on dest.
// Return the amount of found characters.
size_t storage_get_characters(character_t *dest, char *username, size_t max);

// Save character to database.
// On success, 1 is returned. On error, 0.
int storage_create_character(char *username, character_t *src);

#endif
