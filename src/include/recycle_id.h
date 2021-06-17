#ifndef INCLUDE_RECYCLE_ID_H
#define INCLUDE_RECYCLE_ID_H

#include "util.h"

// Example:
// size_t instances[30] = { 0 };
// struct foo[30] = { 0 };
// size_t foo_count = 0;
//
// size_t instance = 0;
//
// foo_count += recycled_id(&instance, instances);
// foo[instance] = free instance to be used.
// recycle_id(instances, instance);

// The usable id will be stored in dest.
// If an id can be reused, 0 will be returned.
// If a new id is created, 1 will be returned.
int recycle_id_get(size_t *dest, size_t *instances);

// Mark id to be reusable/recycled.
// Next time recycle_id_get is called, the
// recycled instances will be returned.
void recycle_id(size_t *instances, size_t id);

#endif
