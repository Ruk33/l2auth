#include <os/memory.h>
#include "mem.h"

// void *mem;

void mem_init()
{
        // size_t reserved = 1024 * 1024 * 10;
        // mem = malloc(reserved);
        // memory_init(mem, reserved);
}

void *mem_alloc(size_t how_much)
{
        // return memory_alloc(mem, how_much);
        return calloc(1, how_much);
}

void mem_free(void *block)
{
        // memory_free(block);
        free(block);
}
