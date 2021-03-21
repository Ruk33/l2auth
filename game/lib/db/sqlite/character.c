#include <headers.h>
#include <character.h>
#include <storage/session.h>
#include <storage/character.h>

struct StorageCharacterMemory {
        struct HashMap *characters_by_account;
        struct List *   characters;
};

typedef struct StorageCharacterMemory storage_handler_h;

void storage_character_init(
        storage_character_t *storage,
        host_alloc           alloc,
        host_dealloc         dealloc)
{
        assert(storage);
        assert(alloc);
        assert(dealloc);

        storage_handler_h *handler = NULL;

        handler                        = alloc(sizeof(*handler));
        handler->characters_by_account = hash_map_create(alloc, dealloc, 50);
        handler->characters            = list_create(alloc, dealloc);

        storage->alloc   = alloc;
        storage->dealloc = dealloc;
        storage->handler = handler;
}

void storage_character_add(
        storage_character_t *storage,
        char *               account,
        size_t               account_size,
        character_t *        character)
{
        assert(storage);
        assert(storage->handler);
        assert(account);
        assert(account_size);
        assert(character);

        storage_handler_h *handler            = NULL;
        struct List *      account_characters = NULL;
        character_t *      character_copy     = NULL;

        handler            = storage->handler;
        account_characters = hash_map_get(
                handler->characters_by_account, account, account_size);
        character_copy = storage->alloc(sizeof(*character_copy));

        memcpy(character_copy, character, sizeof(*character_copy));
        character_copy->level        = 1;
        character_copy->x            = -83968;
        character_copy->y            = 244634;
        character_copy->z            = -3730;
        character_copy->hp           = 42;
        character_copy->mp           = 42;
        character_copy->max_hp       = 42;
        character_copy->max_mp       = 42;
        character_copy->level        = 1;
        character_copy->exp          = 10;
        character_copy->sp           = 10;
        character_copy->cp           = 40;
        character_copy->max_cp       = 40;
        character_copy->p_attack     = 1;
        character_copy->m_attack     = 1;
        character_copy->p_def        = 1;
        character_copy->m_def        = 1;
        character_copy->evasion_rate = 1;
        character_copy->critical_hit = 1;

        if (!account_characters) {
                account_characters =
                        list_create(storage->alloc, storage->dealloc);
                hash_map_set(
                        handler->characters_by_account,
                        account,
                        account_size,
                        account_characters);
        }

        list_add_last(&account_characters, character_copy);
        list_add_last(&handler->characters, character_copy);
}

struct List *storage_character_get(
        storage_character_t *storage,
        char *               account,
        size_t               account_size)
{
        assert(storage);
        assert(storage->handler);
        assert(account);
        assert(account_size);
        storage_handler_h *handler = NULL;
        handler                    = storage->handler;
        return hash_map_get(
                handler->characters_by_account, account, account_size);
}

struct List *storage_character_all_from_session(
        storage_character_t *storage,
        session_t *          session)
{
        assert(storage);
        assert(session);

        return storage_character_get(
                storage, session->username, strlen(session->username) + 1);
}

character_t *storage_character_active_from_session(
        storage_character_t *storage,
        session_t *          session)
{
        assert(storage);
        assert(session);

        unsigned int index = session->selected_character_index;
        struct List *characters =
                storage_character_all_from_session(storage, session);
        struct ListEntry *iterator = NULL;

        if (characters) {
                iterator = list_get_iterator(characters);
        }

        while (iterator) {
                if (index == 0) {
                        return list_get_value(iterator);
                }

                iterator = list_get_next(iterator);
                index -= 1;
        }

        return NULL;
}

size_t storage_character_close_to(
        storage_character_t *storage,
        character_t *        dest,
        size_t               max,
        character_t *        character,
        unsigned int         range)
{
        storage_handler_h *storage_handler = NULL;
        struct ListEntry * iterator        = NULL;
        character_t *      i_character     = NULL;
        position_t         position        = { 0 };
        position_t         i_position      = { 0 };
        size_t             count           = 0;

        assert(storage);
        assert(dest);
        assert(character);

        storage_handler = storage->handler;
        iterator        = list_get_iterator(storage_handler->characters);
        position.x      = character->x;
        position.y      = character->y;
        position.z      = character->z;

        while (iterator && count < max) {
                i_character  = list_get_value(iterator);
                i_position.x = i_character->x;
                i_position.y = i_character->y;
                i_position.z = i_character->z;

                if (position_distance(&position, &i_position) <= range) {
                        dest[count] = *i_character;
                        count += 1;
                }

                iterator = list_get_next(iterator);
        }

        return count;
}

int storage_character_get_by_index(
        character_t *        dest,
        storage_character_t *storage,
        string_t *           account,
        unsigned int         index)
{
        struct List *     characters  = NULL;
        struct ListEntry *i_character = NULL;

        assert(dest);
        assert(storage);
        assert(account);
        assert(account->buf);

        characters =
                storage_character_get(storage, account->buf, account->size);

        if (!characters) {
                return 0;
        }

        i_character = list_get_iterator(characters);

        while (i_character && index) {
                i_character = list_get_next(i_character);
                index -= 1;
        }

        if (!i_character) {
                return 0;
        }

        memcpy(dest, list_get_value(i_character), sizeof(*dest));

        return 1;
}

size_t storage_character_all(
        storage_character_t *storage,
        character_t **       dest,
        size_t               max)
{
        struct ListEntry * characters      = NULL;
        character_t *      character       = NULL;
        storage_handler_h *storage_handler = NULL;

        size_t characters_count = 0;

        assert(storage);
        assert(dest);

        storage_handler = storage->handler;
        characters      = list_get_iterator(storage_handler->characters);

        while (characters_count < max && characters) {
                character              = list_get_value(characters);
                dest[characters_count] = character;
                characters             = list_get_next(characters);
                characters_count += 1;
        }

        return characters_count;
}
