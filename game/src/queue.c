#include <assert.h>
#include "code.h"
#include "queue.h"

struct QueueItem {
        void *data;
        struct QueueItem *next;
};

struct Queue {
        struct QueueItem *head;
};

struct Queue *queue_new
(void)
{
        struct QueueItem *head = NULL;
        struct Queue *queue = NULL;

        head = code_malloc(sizeof(*head));
        head->next = NULL;

        queue = code_malloc(sizeof(*queue));
        queue->head = head;

        return queue;
}

void queue_enqueue
(struct Queue *queue, void *element)
{
        assert(queue);
        assert(element);

        struct QueueItem *new_entry = NULL;
        struct QueueItem *last = NULL;

        new_entry = code_malloc(sizeof(*new_entry));
        new_entry->data = element;
        new_entry->next = NULL;

        last = queue->head;
        while (last->next) last = last->next;
        last->next = new_entry;
}

void *queue_dequeue(struct Queue *queue)
{
        assert(queue);

        struct QueueItem *previous_head = NULL;
        void *data = NULL;

        previous_head = queue->head->next;
        if (!previous_head) return NULL;

        data = previous_head->data;
        queue->head->next = previous_head->next;

        code_mfree(previous_head);

        return data;
}
