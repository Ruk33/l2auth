#ifndef L2AUTH_CIRCULAR_MEMORY_ALLOC_H
#define L2AUTH_CIRCULAR_MEMORY_ALLOC_H

#include <stdlib.h>

/**
 * Circular memory allocation
 * 
 * This is just a simple idea & allocator I had (don't know if there are
 * existing implementations about it).
 * For the time being, I don't know if it's a good idea and if
 * it's going to work well on the wild, please use with
 * extreme care since it could be broken.
 * 
 * The idea is simple, you reserv a block of memory and start requesting
 * chunks. This will start filling the block of memory up. Once it's filled,
 * it will begin from the start of the block and the process is repeated.
 * This means, initial chunks of memory will be overritten.
 *  
 * Pros:
 * - No need for free
 * - Should be safer than calling malloc since you need to pre-allocate
 * - Should be faster
 * 
 * Cons:
 * - It may not work
 * - You have to pre-alloc the memory
 * - You can't ask for chunks bigger than the block of memory reserved
 * 
 * Additional notes
 * 
 * For this allocator, we use 2 variables as metadata:
 * - max_size (size_t): Hold how much memory has been reserved
 * - cursor (size_t): From where should we allocate the next chunk
 */

typedef char circular_memory_space;

size_t circular_memory_alloc_metadata_size();
void circular_memory_alloc_init
(
        circular_memory_space* space,
        size_t to_be_reserved
);
circular_memory_space* circular_memory_alloc
(
        circular_memory_space* space,
        size_t required
);

#endif
