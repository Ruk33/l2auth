#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

/**
 * NOTE, make sure to call this function before using the manager
 * If memory can't be initialized, the program will exit
 */
void memory_manager_init
(void);

/**
 * Alloc how_much memory (thread safe)
 */
void *memory_manager_alloc
(size_t how_much);

/**
 * Dealloc memory (thread safe)
 */
void memory_manager_free
(void *mem);

#endif
