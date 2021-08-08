#ifndef INCLUDE_LIST_H
#define INCLUDE_LIST_H

#define list_each(type, name, arr)                                    \
        for (struct list *list_iterator = (arr)->head;                \
             list_iterator && (name = (type *) list_iterator->value); \
             list_iterator = list_iterator->next)

struct list {
        struct list *next;
        struct list *head;
        void *value;
};

void list_add(struct list *src, void *value);

void list_remove(struct list *src, void *value);

#endif
