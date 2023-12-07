#ifndef LIST_H
#define LIST_H

#define MAX_WORD_SIZE 1024

struct node {
    char *word;
    struct node *next;
};

struct list {
    struct node *head;
    struct node *tail;
};

struct list *init_list();
void add_to_list(struct list *list, const char *word);
void print_list(struct list *list);
void free_list(struct list *list);

#endif /* LIST_H */
