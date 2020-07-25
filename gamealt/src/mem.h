#ifndef MEMORY_H
#define MEMORY_H

#include <os/memory.h>

void mem_init();
void *mem_alloc(size_t how_much);
void mem_free(void *block);

#endif
