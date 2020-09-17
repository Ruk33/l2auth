#include <assert.h>
#include <stdlib.h>
#include <pthread.h>
#include <log/log.h>
#include <os/memory.h>
#include "memory_manager.h"

#define KB(amount) ((amount) * 1024)
#define MB(amount) ((KB(amount)) * 1024)
#define RESERVED_MEMORY MB(50)

static memory *memory_instance = NULL;
static pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;

void memory_manager_init
(void)
{
        memory_instance = calloc(1, RESERVED_MEMORY);

        if (!memory_instance) {
                log_fatal("Could not allocate game server memory");
                exit(EXIT_FAILURE);
        }

        memory_init(memory_instance, RESERVED_MEMORY);
}

void *memory_manager_alloc
(size_t how_much)
{
        assert(memory_instance);
        assert(how_much > 0);

        void *mem = NULL;

        pthread_mutex_lock(&thread_mutex);
        mem = memory_alloc(memory_instance, how_much);
        pthread_mutex_unlock(&thread_mutex);

        return mem;
}

void memory_manager_free
(void *mem)
{
        assert(memory_instance);
        assert(mem);

        pthread_mutex_lock(&thread_mutex);
        memory_free(mem);
        pthread_mutex_unlock(&thread_mutex);
}

void memory_manager_finish
(void)
{
        free(memory_instance);
        memory_instance = NULL;
}
