#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "memory_cb.h"

struct Hashtable;

struct Hashtable *hashtable_new
(memory_cb_alloc m, memory_cb_free f);

void hashtable_put
(struct Hashtable *ht, int key, void *value);

void *hashtable_get
(struct Hashtable *ht, int key);

void hashtable_remove
(struct Hashtable *ht, int key);

#endif
