#include <stdbool.h>

#ifndef LIST_H
#define LIST_H

struct node {
    char *word;
    struct node *next;
};

struct node *list_init();
void list_add(struct node **tail);
void list_term(struct node *, bool);

#endif /* LIST_H */
