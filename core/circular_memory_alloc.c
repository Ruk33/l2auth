#ifndef L2AUTH_CIRCULAR_MEMORY_ALLOC_C
#define L2AUTH_CIRCULAR_MEMORY_ALLOC_C

#include <stdlib.h>
#include <string.h>

/**
 * Circular memory allocation
 * 
 * This is just a simple idea & allocator I had (don't know if there are
 * existing implementations about it).
 * For the time being, I don't know if it's a good idea and if
 * it's going to work well on the wild, please use with
 * extreme care since it could be broken.
 * 
 * The idea is simple, you reserv a block of memory and start requesting
 * chunks. This will start filling the block of memory up. Once it's filled,
 * it will begin from the start of the block and the process is repeated.
 * This means, initial chunks of memory will be overritten.
 *  
 * Pros:
 * - No need for free
 * - Should be safer than calling malloc since you need to pre-allocate
 * - Should be faster
 * 
 * Cons:
 * - It may not work
 * - You have to pre-alloc the memory
 * - You can't ask for chunks bigger than the block of memory reserved
 * 
 * Additional notes
 * 
 * For this allocator, we use 2 variables as metadata:
 * - max_size (size_t): Hold how much memory has been reserved
 * - cursor (size_t): From where should we allocate the next chunk
 */

typedef char circular_memory_space;

void circular_memory_alloc_set_max_space
(
        circular_memory_space* space,
        size_t max_space
)
{
        memcpy(space, &max_space, sizeof(max_space));
}

size_t circular_memory_alloc_max_space(circular_memory_space* space)
{
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
        memcpy(space + sizeof(size_t), &new_cursor, sizeof(new_cursor));
}

size_t circular_memory_alloc_cursor(circular_memory_space* space)
{
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
        size_t max_size_metadata = sizeof(size_t);
        size_t cursor_metadata = sizeof(size_t);
        size_t metadata_header = max_size_metadata + cursor_metadata;
        circular_memory_space* block = space + metadata_header + cursor;

        memset(block, 0, block_memory);
        return block;
}

circular_memory_space* circular_memory_alloc_reserve_space
(
        size_t to_be_reserved
)
{
        size_t max_size_metadata = sizeof(size_t);
        size_t cursor_metadata = sizeof(size_t);
        size_t metadata_space = max_size_metadata + cursor_metadata;
        size_t reserved_plus_metadata_space = to_be_reserved + metadata_space;
        circular_memory_space* space = calloc(reserved_plus_metadata_space, 1);
        circular_memory_alloc_set_max_space(space, to_be_reserved);
        return space;
}

circular_memory_space* circular_memory_alloc
(
        circular_memory_space* space,
        size_t required
)
{
        size_t max_space = circular_memory_alloc_max_space(space);
        size_t cursor = circular_memory_alloc_cursor(space);
        size_t new_cursor = 0;

        new_cursor = cursor + required;

        if (new_cursor > max_space) {
                new_cursor = 0;
        }

        circular_memory_alloc_set_cursor(space, new_cursor);
        return circular_memory_alloc_block(space, cursor, required);
}

#endif
