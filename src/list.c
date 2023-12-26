#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "buffer.h"
#include "debug.h"
#include "list.h"

struct list *init_list()
{
    struct list *ls;

    TRACE("[LIST] Initilize.\n");

    ls = malloc(sizeof(struct list));
    if (ls == NULL) {
        perror("Failed to allocate memory for ls.");
        exit(EXIT_FAILURE);
    }

    ls->head = ls->tail = NULL;

    return ls;
}

void add_to_list(struct list *ls, struct buffer *buff)
{
    struct node *new_node;

    if (ls == NULL) {
        TRACE("[LIST] Failed to add to list: NULL pointer received.\n");
        return;
    }

    if (buff == NULL) {
        TRACE("[LIST] Failed to add to list: NULL pointer received for buffer.\n");
        return;
    }

    if (buff->length == 0) {
        TRACE("[LIST] Failed to add new word: word is empty.\n");
        return;
    }

    TRACE("[LIST] Add '%s'.\n", buff->store);

    new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        perror("Failed to allocate memory for new node.");
        return;
    }

    new_node->token = malloc(buff->length + 1);
    if (new_node == NULL) {
        perror("Failed to allocate memory for new token.");
        free(new_node);
        return;
    }

    new_node->next = NULL;
    strcpy(new_node->token, buff->store);

    if (ls->head == NULL) {
        ls->head = ls->tail = new_node;
    } else {
        ls->tail->next = new_node;
        ls->tail = new_node;
    }
}

void empty_list(struct list *ls)
{
    struct node *curr;
    struct node *next;

    TRACE("[LIST] Empty.\n");

    if (ls == NULL) {
        fputs("Failed to empty list: NULL pointer received.\n", stderr);
        return;
    }

    curr = ls->head;
    while (curr != NULL) {
        next = curr->next;
        TRACE("\tFree node '%s'.\n", curr->token);
        free(curr->token);
        free(curr);
        curr = next;
    }

    ls->head = NULL;
}

void free_list(struct list *ls)
{
    TRACE("[LIST] Free.\n");

    if (ls == NULL) {
        fputs("Failed to free list: NULL pointer received.\n", stderr);
        return;
    }

    empty_list(ls);
    free(ls);
}

void for_each_node(struct list *ls, void (*handler)(struct node *))
{
    struct node *curr;

    if (ls == NULL) {
        fputs("Failed to iterate over list: NULL pointer received.\n", stderr);
        return;
    }

    for (curr = ls->head; curr != NULL; curr = curr->next) {
        (*handler)(curr);
    }
}
