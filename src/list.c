#include <assert.h>
#include "include/list.h"

void list_add(struct list *src, void *value)
{
        struct list *free_node = 0;

        src->last = src->last ? src->last : src;
        free_node = src + 1;

        while (free_node->value) {
                free_node += 1;
        }

        free_node->value = value;
        free_node->next  = 0;
        src->last->next  = free_node;
        src->last        = free_node;
}

void list_remove(struct list *src, void *value)
{
        struct list **match    = 0;
        struct list *to_remove = 0;

        match = &src;

        while (*match && (*match)->value != value) {
                match = &(*match)->next;
        }

        to_remove  = *match;
        *match     = (*match)->next;
        *to_remove = (struct list){ 0 };
}
