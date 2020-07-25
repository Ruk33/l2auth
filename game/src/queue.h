#ifndef QUEUE_H
#define QUEUE_H

struct Queue;

struct Queue *queue_new();
void queue_add(struct Queue *queue, void *elem);
void *queue_pop(struct Queue *queue);

#endif
