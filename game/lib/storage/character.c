#include "../headers.h"
#include "session.h"
#include "character.h"

void storage_character_init(storage_character_t *storage, host_alloc alloc, host_dealloc dealloc)
{
        assert(storage);
        assert(alloc);
        assert(dealloc);
        storage->alloc = alloc;
        storage->dealloc = dealloc;
        storage->characters = hash_map_create(alloc, dealloc, 50);
}

void storage_character_add(
        storage_character_t *storage,
        char *account,
        size_t account_size,
        character_t *character
)
{
        assert(storage);
        assert(storage->characters);
        assert(account);
        assert(account_size);
        assert(character);

        struct List *account_characters = hash_map_get(
                storage->characters,
                account,
                account_size
        );
        character_t *character_copy = storage->alloc(sizeof(*character_copy));

        memcpy(character_copy, character, sizeof(*character_copy));
        character_copy->id = 42;
        character_copy->level = 1;
        character_copy->x = -83968;
        character_copy->y = 244634;
        character_copy->z = -3730;
        character_copy->hp = 42;
        character_copy->mp = 42;
        character_copy->max_hp = 42;
        character_copy->max_mp = 42;
        character_copy->level = 1;
        character_copy->exp = 10;
        character_copy->sp = 10;
        character_copy->cp = 40;
        character_copy->max_cp = 40;
        character_copy->p_attack = 1;
        character_copy->m_attack = 1;
        character_copy->p_def = 1;
        character_copy->m_def = 1;
        character_copy->evasion_rate = 1;
        character_copy->critical_hit = 1;

        if (!account_characters)
        {
                account_characters = list_create(storage->alloc, storage->dealloc);
                hash_map_set(
                        storage->characters,
                        account,
                        account_size,
                        account_characters
                );
        }

        list_add_last(&account_characters, character_copy);
}

struct List *storage_character_get(
        storage_character_t *storage,
        char *account,
        size_t account_size
)
{
        assert(storage);
        assert(storage->characters);
        assert(account);
        assert(account_size);
        return hash_map_get(storage->characters, account, account_size);
}

struct List *storage_character_all_from_session(
        storage_character_t *storage,
        session_t *session
)
{
        assert(storage);
        assert(session);

        return storage_character_get(
                storage,
                session->username,
                strlen(session->username) + 1
        );
}

character_t *storage_character_active_from_session(
        storage_character_t *storage,
        session_t *session
)
{
        assert(storage);
        assert(session);

        int index = session->selected_character_index;
        struct List *characters = storage_character_all_from_session(
                storage,
                session
        );
        struct ListEntry *iterator = NULL;

        if (characters)
        {
                iterator = list_get_iterator(characters);
        }

        while (iterator)
        {
                if (index == 0)
                {
                        return list_get_value(iterator);
                }

                iterator = list_get_next(iterator);
                index -= 1;
        }

        return NULL;
}
