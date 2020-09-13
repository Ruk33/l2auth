#ifndef QUEUE_H
#define QUEUE_H

struct Queue;

struct Queue *queue_new
(void);

void queue_enqueue
(struct Queue *queue, void *element);

void *queue_dequeue
(struct Queue *queue);

#endif
