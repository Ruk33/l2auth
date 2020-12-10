#ifndef STORAGE_CHARACTER_H
#define STORAGE_CHARACTER_H

#include "../headers.h"
#include "../character.h"
#include "session.h"

struct StorageCharacter {
        host_alloc alloc;
        host_dealloc dealloc;
        struct HashMap *characters;
};

typedef struct StorageCharacter storage_character_t;

/**
 * This function must be called before
 * using any of the others.
 */
void storage_character_init(storage_character_t *storage, host_alloc alloc, host_dealloc dealloc);

/**
 * Add a new character to an account.
 */
void storage_character_add(
        storage_character_t *storage,
        char *account,
        size_t account_size,
        character_t *character
);

/**
 * Get all the characters from
 * account. If no characters are found,
 * NULL will be returned.
 */
struct List *storage_character_get(
        storage_character_t *storage,
        char *account,
        size_t account_size
);

/**
 * Get all characters associated to session.
 */
struct List *storage_character_all_from_session(
        storage_character_t *storage,
        session_t *session
);

/**
 * Get active character from session.
 * Session's selected character index must be set.
 * NULL can be returned if no character is found.
 */
character_t *storage_character_active_from_session(
        storage_character_t *storage,
        session_t *session
);

#endif
