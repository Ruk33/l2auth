#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <core/memory.h>

void memory_init(memory* mem, size_t reserved)
{
        assert(mem);
        assert(reserved);
        memset(mem, 0, reserved);
}

memory* memory_alloc(memory* from, size_t how_much)
{
        assert(from);
        assert(how_much);

        memory* tail = from;
        unsigned char blocks = ((how_much + 1) / 4096) >> 2;
        blocks = blocks == 0 ? 1 : blocks;

        while (*tail) tail += (unsigned char) (*tail) * 16384;

        *tail = blocks;
        return tail + 1;
}

void memory_free(memory* block)
{
        assert(block);
        *(block - 1) = 0;

        /*
         * (franco.montenegro) Uncomment the following to easier debug
         * it will make sure all data gets reset
         * size_t reserved = memory_alloc_reserved_by_block(block);
         * memset(block - sizeof(unsigned char), 0, reserved + sizeof(unsigned char));
         */
}
