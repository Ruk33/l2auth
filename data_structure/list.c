#include <assert.h>
#include "memory_cb.h"
#include "list.h"

struct List {
        memory_cb_alloc malloc;
        memory_cb_free free;
        struct ListEntry *first;
};

struct ListEntry {
        void *value;
        struct ListEntry *next;
};

struct List *list_new
(memory_cb_alloc m, memory_cb_free f)
{
        struct List *list = NULL;

        list = m(sizeof(*list));
        list->malloc = m;
        list->free = f;
        list->first = NULL;

        return list;
}

void list_add
(struct List *list, void *element)
{
        assert(list);

        struct ListEntry *last = NULL;
        struct ListEntry *new_node = NULL;

        new_node = list->malloc(sizeof(*new_node));
        new_node->value = element;
        new_node->next = NULL;

        if (!list->first) {
                list->first = new_node;
                return;
        }

        last = list->first;
        while (last->next) last = last->next;
        last->next = new_node;
}

void list_remove
(struct List *list, void *element)
{
        assert(list);

        struct ListEntry *prev = NULL;
        struct ListEntry *node = NULL;

        node = list->first;

        while (node) {
                if (node->value == element) break;
                prev = node;
                node = node->next;
        }

        if (!node) return;
        if (prev) prev->next = node->next;
        if (list->first == node) list->first = node->next;

        list->free(node);
}

struct ListEntry *list_first
(struct List *list)
{
        assert(list);
        return list->first;
}

struct ListEntry *list_entry_next
(struct ListEntry *entry)
{
        return entry ? entry->next : NULL;
}

void *list_entry_value
(struct ListEntry *entry)
{
        return entry ? entry->value : NULL;
}

void list_free
(struct List *list)
{
        assert(list);

        memory_cb_free f = NULL;

        f = list->free;
        f(list);
}
