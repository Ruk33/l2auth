#include <assert.h>
#include "memory_cb.h"
#include "list.h"
#include "queue.h"

struct Queue {
        memory_cb_alloc malloc;
        memory_cb_free free;
        struct List *elements;
};

struct Queue *queue_new
(memory_cb_alloc m, memory_cb_free f)
{
        struct Queue *queue = NULL;

        queue = m(sizeof(*queue));
        queue->malloc = m;
        queue->free = f;
        queue->elements = list_new(m, f);

        return queue;
}

void queue_enqueue
(struct Queue *queue, void *element)
{
        assert(queue);
        assert(queue->elements);
        list_add(queue->elements, element);
}

void *queue_dequeue
(struct Queue *queue)
{
        assert(queue);
        assert(queue->elements);

        struct ListEntry *head = NULL;
        void *element = NULL;

        head = list_first(queue->elements);
        element = list_entry_value(head);
        list_remove(queue->elements, element);

        return element;
}

void queue_free
(struct Queue *queue)
{
        assert(queue);
        assert(queue->elements);

        memory_cb_free f = NULL;

        f = queue->free;

        list_free(queue->elements);
        f(queue);
}
