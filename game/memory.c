#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memory.h"

#define MAX_ALLOCATIONS ((size_t) 256)
#define MAX_ALLOCATION_PER_BLOCK ((size_t) 65536)

// +1 since it's metadata
// used by the allocator to know
// if a particular block of
// memory is being used.
#define ALLOCATION_PER_BLOCK ((size_t) (MAX_ALLOCATION_PER_BLOCK + 1))

static unsigned char *memory_head = NULL;

static int is_block_used(unsigned char *block)
{
        assert(block);
        return block[0] != 0;
}

static void mark_block_as_used(unsigned char *block)
{
        assert(block);
        block[0] = 1;
}

int memory_init()
{
        memory_head = calloc(MAX_ALLOCATIONS * ALLOCATION_PER_BLOCK, 1);
        return memory_head != NULL;
}

void memory_print_stats()
{
        unsigned char *block = memory_head;
        size_t used = 0;

        for (size_t i = 0; i < MAX_ALLOCATIONS; i++) {
                if (is_block_used(block)) {
                        used += 1;
                }
                block += ALLOCATION_PER_BLOCK;
        }

        printf("Memory block: %ld bytes.\n", MAX_ALLOCATIONS * ALLOCATION_PER_BLOCK);
        printf("Memory per allocation: %ld bytes.\n", ALLOCATION_PER_BLOCK);
        printf("Memory used: %ld bytes.\n", used * ALLOCATION_PER_BLOCK);
        printf("Memory left: %ld bytes.\n", (MAX_ALLOCATIONS - used) * ALLOCATION_PER_BLOCK);
        printf("Memory allocations left: %ld.\n", MAX_ALLOCATIONS - used);
}

void *memory_alloc(size_t n)
{
        assert(memory_head);
        assert(n <= MAX_ALLOCATION_PER_BLOCK);

        unsigned char *block = memory_head;

        while (is_block_used(block)) {
                block += ALLOCATION_PER_BLOCK;
        }

        assert(!is_block_used(block));
        mark_block_as_used(block);

        return block + 1;
}

void memory_free(void *memory)
{
        if (!memory) {
                return;
        }

        unsigned char *block = (unsigned char *) memory - 1;
        memset(block, 0, ALLOCATION_PER_BLOCK);
}
