#ifndef L2AUTH_MEMORY_H
#define L2AUTH_MEMORY_H

#include <stdlib.h>

/**
 * Memory
 * 
 * The only purpose of this module is to preallocate memory
 * and properly handle it instead of having to manually call
 * malloc. The main reason for this is to preallocate a fixed 
 * amount of memory for each client and prevent running out of 
 * resources because of memory leaks allowing to kill just the client
 * (in case they ask for more memory than what it was given) instead
 * of the whole process.
 */

typedef char memory;

/**
 * Initialize the library in the block of memory
 * 
 * size_t mem_to_be_reserved = 1024;
 * memory* mem = malloc(mem_to_be_reserved);
 * memory_init(mem, mem_to_be_reserved);
 */
void memory_init(memory* mem, size_t reserved);

void* memory_alloc(memory* from, size_t how_much);

void memory_free(void* block);

#endif
