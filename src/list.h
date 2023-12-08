#ifndef LIST_H
#define LIST_H

#include "buffer.h"

struct node {
    char *word;
    struct node *next;
};

struct list {
    struct node *head;
    struct node *tail;
};

struct list *init_list();
void add_to_list(struct list *, const struct buffer *);
void empty_list(struct list *);
void free_list(struct list *);
void for_each_node(struct list *, void (*)(struct node *));

#endif /* LIST_H */
