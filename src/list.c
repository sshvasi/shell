#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

struct list *init_list()
{
    struct list *list;
    if ((list = malloc(sizeof(struct list))) == NULL) {
        perror("Failed to allocate memory for list.");
        return NULL;
    }

    list->head = list->tail = NULL;

    return list;
}

void add_to_list(struct list *list, const char *word)
{
    if (list == NULL) {
        fputs("Failed to add to list: NULL pointer received.\n", stderr);
        return;
    }

    int word_size = strlen(word);
    if (word_size >= MAX_WORD_SIZE) {
        fprintf(stderr,
                "Failed to add new word: not enough size. "
                "Max: %d, provided: %d.\n",
                MAX_WORD_SIZE, word_size);
        return;
    }
    if (word_size == 0) {
        /* fprintf(stderr, "Failed to add new word: word is empty.\n"); */
        return;
    }

    struct node *new_node;
    if ((new_node = malloc(sizeof(struct node))) == NULL) {
        perror("Failed to allocate memory for new node.");
        return;
    }


    if ((new_node->word = malloc(word_size + 1)) == NULL) {
        free(new_node);
        perror("Failed to allocate memory for new word.");
        return;
    }

    new_node->next = NULL;
    strcpy(new_node->word, word);

    if (list->head == NULL) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
}

void free_list(struct list *list)
{
    if (list == NULL) {
        fputs("Failed to free list: NULL pointer received.\n", stderr);
        return;
    }

    struct node *current = list->head;
    struct node *next;

    while (current != NULL) {
        next = current->next;
        free(current->word);
        free(current);
        current = next;
    }

    free(list);
}

void print_list(struct list *list)
{
    if (list == NULL) {
        fputs("Failed to print list: NULL pointer received.\n", stderr);
        return;
    }

    struct node *iter;

    for (iter = list->head; iter; iter = iter->next)
        printf("[%s]\n", iter->word);
}
