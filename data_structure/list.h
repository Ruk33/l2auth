#ifndef LIST_H
#define LIST_H

#include "memory_cb.h"

struct List;

struct ListEntry;

struct List *list_new
(memory_cb_alloc m, memory_cb_free f);

void list_add
(struct List *list, void *element);

void list_remove
(struct List *list, void *element);

struct ListEntry *list_first
(struct List *list);

struct ListEntry *list_entry_next
(struct ListEntry *entry);

void *list_entry_value
(struct ListEntry *entry);

void list_free
(struct List *list);

#endif
