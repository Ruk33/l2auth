#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

/**
 * This function must be called
 * before using ANY of the others.
 * On success, 1 is returned.
 * On error, 0 will be returned.
 */
int memory_init();

/**
 * Print memory allocator stats.
 * How much memory is being used
 * and how much is left.
 */
void memory_print_stats();

/**
 * Allocate n bytes of memory.
 * Returned bytes are zeroed.
 */
void *memory_alloc(size_t n);

/**
 * Deallocate memory. NULL won't break
 * this function.
 * Deallocated memory is zeroed.
 */
void memory_free(void *memory);

#endif
