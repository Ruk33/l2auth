#ifndef MEMORY_CB_H
#define MEMORY_CB_H

#include <stdlib.h>

typedef void *(* memory_cb_alloc)
(size_t how_much);

typedef void (* memory_cb_free)
(void *memory);

#endif
