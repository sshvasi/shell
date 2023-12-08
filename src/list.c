#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "buffer.h"
#include "debug.h"
#include "list.h"

struct list *init_list()
{
    struct list *list;
    if ((list = malloc(sizeof(struct list))) == NULL) {
        perror("Failed to allocate memory for list.");
        exit(EXIT_FAILURE);
    }

    list->head = list->tail = NULL;

    TRACE("[LIST] Initilize.\n");

    return list;
}

void add_to_list(struct list *list, const struct buffer *buff)
{
    TRACE("[LIST] Add '%s'.\n", buff->store);
    if (list == NULL) {
        fputs("Failed to add to list: NULL pointer received.\n", stderr);
        return;
    }

    if (buff->length == 0) {
        TRACE("[LIST] Failed to add new word: word is empty.\n");
        return;
    }

    struct node *new_node;
    if ((new_node = malloc(sizeof(struct node))) == NULL) {
        perror("Failed to allocate memory for new node.");
        exit(EXIT_FAILURE);
    }


    if ((new_node->word = malloc(buff->length)) == NULL) {
        free(new_node);
        perror("Failed to allocate memory for new word.");
        exit(EXIT_FAILURE);
    }

    new_node->next = NULL;
    strcpy(new_node->word, buff->store);

    if (list->head == NULL) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
}

void empty_list(struct list *list)
{
    TRACE("[LIST] Empty.\n");
    if (list == NULL) {
        fputs("Failed to empty list: NULL pointer received.\n", stderr);
        return;
    }

    struct node *curr = list->head;
    struct node *next;

    while (curr != NULL) {
        next = curr->next;
        TRACE("[LIST] Free node '%s'.\n", curr->word);
        free(curr->word);
        free(curr);
        curr = next;
    }

    list->head = NULL;
}

void free_list(struct list *list)
{
    TRACE("[LIST] Free.\n");
    if (list == NULL) {
        fputs("Failed to free list: NULL pointer received.\n", stderr);
        return;
    }

    empty_list(list);
    free(list);
}

void for_each_node(struct list *list, void (*handler)(struct node *))
{
    TRACE("[LIST] For each.\n");
    if (list == NULL) {
        fputs("Failed to iterate over list: NULL pointer received.\n", stderr);
        return;
    }

    struct node *iter;
    for (iter = list->head; iter; iter = iter->next) {
        (*handler)(iter);
    }
}
