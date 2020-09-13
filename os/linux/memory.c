#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <os/memory.h>

static unsigned char memory_blocks_for_mem(size_t how_much)
{
        unsigned char blocks = (unsigned char) (((how_much + 1) / 4096) >> 2);
        return blocks == 0 ? 1 : blocks;
}

void memory_init(memory* mem, size_t reserved)
{
        assert(mem);
        assert(reserved);

        memset(mem, 0, reserved);

        // First byte tell us how many blocks are available
        *((unsigned char *) mem) = memory_blocks_for_mem(reserved);
}

void* memory_alloc(memory* from, size_t how_much)
{
        // assert(from);
        // assert(how_much > 0);
        // return calloc(1, how_much);

        assert(from);
        assert(how_much);

        memory* head = from + sizeof(unsigned char);
        memory* block = head;

        unsigned char max_blocks = (unsigned char) *from;
        unsigned char required_blocks = memory_blocks_for_mem(how_much);

        while (*block) {
                block += (unsigned char) (*block) * 16384;
                max_blocks--;
        }

        assert(max_blocks >= required_blocks);

        memset(block, 0, required_blocks * 16384);

        // First byte stores how many blocks were used for this alloc 
        *((unsigned char *) block) = required_blocks;
        return block + sizeof(unsigned char);
}

void memory_free(void* block)
{
        // assert(block);
        // free(block);

        assert(block);

        // This chunk of alloc memory no longer uses any blocks
        memory* block_as_mem = (memory *) block;
        *(block_as_mem - sizeof(unsigned char)) = 0;
}
