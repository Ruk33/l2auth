#ifndef QUEUE_H
#define QUEUE_H

#include "memory_cb.h"

struct Queue;

struct Queue *queue_new
(memory_cb_alloc m, memory_cb_free f);

void queue_enqueue
(struct Queue *queue, void *element);

void *queue_dequeue
(struct Queue *queue);

void queue_free
(struct Queue *queue);

#endif
