#ifndef L2AUTH_CIRCULAR_MEMORY_ALLOC_C
#define L2AUTH_CIRCULAR_MEMORY_ALLOC_C

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/circular_memory_alloc.h>

size_t circular_memory_alloc_metadata_size()
{
        size_t max_space_metadata = sizeof(size_t);
        size_t cursor_metadata = sizeof(size_t);
        return max_space_metadata + cursor_metadata;
}

void circular_memory_alloc_set_max_space
(
        circular_memory_space* space,
        size_t max_space
)
{
        assert(space);
        memcpy(space, &max_space, sizeof(max_space));
}

size_t circular_memory_alloc_max_space(circular_memory_space* space)
{
        assert(space);

        size_t max_space = 0;
        memcpy(&max_space, space, sizeof(max_space));

        return max_space;
}

void circular_memory_alloc_set_cursor
(
        circular_memory_space* space,
        size_t new_cursor
)
{
        assert(space);
        memcpy(space + sizeof(size_t), &new_cursor, sizeof(new_cursor));
}

size_t circular_memory_alloc_cursor(circular_memory_space* space)
{
        assert(space);

        size_t cursor = 0;
        memcpy(&cursor, space + sizeof(size_t), sizeof(cursor));

        return cursor;
}

circular_memory_space* circular_memory_alloc_block
(
        circular_memory_space* space,
        size_t cursor,
        size_t block_memory
)
{
        assert(space);

        size_t metadata_header = circular_memory_alloc_metadata_size();
        circular_memory_space* block = space + metadata_header + cursor;

        memset(block, 0, block_memory);

        return block;
}

void circular_memory_alloc_init
(
        circular_memory_space* space,
        size_t to_be_reserved
)
{
        assert(space);
        assert(to_be_reserved);

        size_t metadata = circular_memory_alloc_metadata_size();
        assert(to_be_reserved - metadata > 0);

        size_t free_space = to_be_reserved - metadata;

        memset(space, 0, to_be_reserved);
        circular_memory_alloc_set_max_space(space, free_space);
}

circular_memory_space* circular_memory_alloc
(
        circular_memory_space* space,
        size_t required
)
{
        assert(space);
        assert(required);

        size_t max_space = circular_memory_alloc_max_space(space);
        size_t cursor = circular_memory_alloc_cursor(space);
        size_t new_cursor = 0;

        log_info("Max mem: %ld, asked: %ld", max_space, required);
        assert(max_space >= required);

        new_cursor = cursor + required;

        if (new_cursor > max_space) {
                cursor = 0;
                new_cursor = required;
        }

        circular_memory_alloc_set_cursor(space, new_cursor);
        return circular_memory_alloc_block(space, cursor, required);
}

#endif
