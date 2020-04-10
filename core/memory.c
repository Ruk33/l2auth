#ifndef L2AUTH_MEMORY_C
#define L2AUTH_MEMORY_C

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <core/memory.h>

void memory_init_block(memory* block, size_t reserved)
{
        assert(block);
        assert(reserved);
        size_t reserved_metadata = sizeof(size_t);
        size_t is_used_metadata = sizeof(unsigned char);
        assert(reserved - reserved_metadata - is_used_metadata > 0);
        size_t real_free_space = reserved - reserved_metadata - is_used_metadata;
        memcpy(block, &real_free_space, sizeof(real_free_space));
        memset(block + reserved_metadata, 0, is_used_metadata + real_free_space);
}

void memory_init(memory* mem, size_t reserved)
{
        assert(mem);
        assert(reserved);
        size_t max_metadata = sizeof(size_t);
        assert(reserved - max_metadata > 0);
        size_t real_free_space = reserved - max_metadata;
        memcpy(mem, &real_free_space, sizeof(real_free_space));
        memory_init_block(mem + max_metadata, real_free_space);
}

size_t memory_reserved(memory* block)
{
        assert(block);
        size_t max = 0;
        memcpy(&max, block, sizeof(max));
        return max;
}

size_t memory_reserved_by_block(memory* block)
{
        assert(block);
        size_t reserved = 0;
        size_t is_used_metadata = sizeof(unsigned char);
        memcpy(
                &reserved,
                block - is_used_metadata - sizeof(size_t),
                sizeof(reserved)
        );
        return reserved;
}

int memory_is_block_being_used(memory* block)
{
        assert(block);
        return (int) *(block - sizeof(unsigned char));
}

memory* memory_alloc(memory* from, size_t how_much)
{
        assert(from);
        assert(how_much);

        size_t max = memory_reserved(from);
        assert(max - how_much > 0);

        size_t max_metadata = sizeof(size_t);
        size_t reserved_metadata = sizeof(size_t);
        size_t is_used_metadata = sizeof(unsigned char);
        size_t metadata = max_metadata + reserved_metadata + is_used_metadata;

        size_t how_much_with_metadata =
                how_much + reserved_metadata + is_used_metadata;

        size_t start = 0;
        size_t end = metadata;

        size_t start_end_mem = 0;
        size_t mem_in_block = 0;

        memory* block;

        while (end < max) {
                block = from + end;
                mem_in_block = memory_reserved_by_block(block);

                if (start_end_mem == 0) start = end;
                else start_end_mem += reserved_metadata + is_used_metadata;

                end += reserved_metadata + is_used_metadata + mem_in_block;
                if (memory_is_block_being_used(block)) {
                        start_end_mem = 0;
                        continue;
                }
                start_end_mem += mem_in_block;

                if (start_end_mem - how_much_with_metadata < 10) {
                        memory_init_block(
                                from + start - reserved_metadata - is_used_metadata,
                                start_end_mem
                        );
                        *(from + start - is_used_metadata) = 1;
                        return from + start;
                }

                if (start_end_mem > how_much_with_metadata) {
                        memory_init_block(
                                from + start - reserved_metadata - is_used_metadata,
                                how_much_with_metadata
                        );
                        memory_init_block(
                                from + start + how_much,
                                start_end_mem - how_much
                        );
                        *(from + start - is_used_metadata) = 1;
                        return from + start;
                }
        }

        assert(0);
        return NULL;
}

void memory_free(memory* block)
{
        assert(block);
        *(block - sizeof(unsigned char)) = 0;

        /*
         * (franco.montenegro) Uncomment the following to easier debug
         * it will make sure all data gets reset
         * size_t reserved = memory_alloc_reserved_by_block(block);
         * memset(block - sizeof(unsigned char), 0, reserved + sizeof(unsigned char));
         */
}

#endif
