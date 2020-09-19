#include <assert.h>
#include <string.h>
#include "memory_cb.h"
#include "list.h"
#include "hashtable.h"

struct HashtableEntry {
        int key;
        void *value;
};

struct Hashtable {
        memory_cb_alloc malloc;
        memory_cb_free free;
        struct List *entries;
};

struct Hashtable *hashtable_new
(memory_cb_alloc m, memory_cb_free f)
{
        struct Hashtable *ht = NULL;

        ht = m(sizeof(*ht));
        ht->malloc = m;
        ht->free = f;
        ht->entries = list_new(m, f);

        return ht;
}

void hashtable_put
(struct Hashtable *ht, int key, void *value)
{
        assert(ht);
        assert(key);

        struct HashtableEntry *entry = NULL;

        entry = ht->malloc(sizeof(*entry));
        entry->key = key;
        entry->value = value;

        list_add(ht->entries, entry);
}

static struct HashtableEntry *hashtable_get_entry
(struct Hashtable *ht, int key)
{
        assert(ht);
        assert(key);

        struct ListEntry *list_entry = NULL;
        struct HashtableEntry *entry = NULL;
        
        list_entry = list_first(ht->entries);

        while (list_entry) {
                entry = list_entry_value(list_entry);
                if (entry->key == key) break;
                list_entry = list_entry_next(list_entry);
        }

        return entry;
}

void *hashtable_get
(struct Hashtable *ht, int key)
{
        assert(ht);

        struct HashtableEntry *entry = NULL;

        entry = hashtable_get_entry(ht, key);

        if (!entry) return NULL;

        return entry->value;
}

void hashtable_remove
(struct Hashtable *ht, int key)
{
        assert(ht);

        struct HashtableEntry *entry = NULL;

        entry = hashtable_get_entry(ht, key);

        if (!entry) return;

        list_remove(ht->entries, entry);
        ht->free(entry);
}
