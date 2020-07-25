#include <assert.h>
#include "mem.h"
#include "queue.h"

struct Queue {
        void *elem;
        struct Queue *next;
};

struct Queue *queue_new()
{
        struct Queue *queue = mem_alloc(sizeof(struct Queue));
        queue->elem = NULL;
        queue->next = NULL;
        return queue;
}

void queue_add(struct Queue *queue, void *elem)
{
        assert(queue);

        struct Queue *last = queue;
        struct Queue *new = queue->elem ? queue_new() : queue;
        new->elem = elem;
        while (last->next) last = last->next;
        last->next = new;
}

void *queue_pop(struct Queue *queue)
{
        assert(queue);
        
        void *elem = queue->elem;
        struct Queue *next = queue->next ? queue->next : queue;

        queue->elem = next->elem;
        queue->next = next->next;

        if (next && queue != next) mem_free(next);

        return elem;
}
