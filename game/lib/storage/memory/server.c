#include <headers.h>
#include <storage/memory/session.h>
#include <storage/memory/character.h>
#include <storage/memory/server.h>

void storage_server_init(storage_server_t *storage, host_alloc alloc_memory, host_dealloc dealloc_memory)
{
        assert(storage);
        assert(alloc_memory);
        assert(dealloc_memory);

        storage_session_init(&storage->session_storage, alloc_memory, dealloc_memory);
        storage_character_init(&storage->character_storage, alloc_memory, dealloc_memory);
}
